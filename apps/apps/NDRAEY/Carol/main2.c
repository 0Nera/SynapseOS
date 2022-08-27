#include <stdio.h>
#include <vesa.h>
#include "animator.h"
#include "dukelib.h"
#include <ports.h>

char* files[] = {
    "/initrd/res/Carol/push/carol_pushing_0.png.duke",
    "/initrd/res/Carol/push/carol_pushing_1.png.duke",
    "/initrd/res/Carol/push/carol_pushing_2.png.duke",
    "/initrd/res/Carol/push/carol_pushing_3.png.duke",
    "/initrd/res/Carol/push/carol_pushing_4.png.duke",
    "/initrd/res/Carol/push/carol_pushing_5.png.duke",
    "/initrd/res/Carol/push/carol_pushing_6.png.duke",
    "/initrd/res/Carol/push/carol_pushing_7.png.duke",
    "/initrd/res/Carol/push/carol_pushing_8.png.duke",
    "/initrd/res/Carol/push/carol_pushing_9.png.duke"
};

char* idle_files[] = {
    "/initrd/res/Carol/idle/carol_idlepose1_0.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_1.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_2.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_3.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_4.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_5.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_6.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_7.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_8.png.duke",
    "/initrd/res/Carol/idle/carol_idlepose1_9.png.duke"
};


int filescount = 10;

void rect(unsigned int sx, unsigned int sy, unsigned int ex, unsigned int ey) {
    // printf("RECT! \n");
    for(int i=sy; i<ey; i++) {
        for(int j=sx; j<ex; j++) {
            draw_pixel(j, i, 0);
        }
    }
}

int main() {
    printf("CarolDemo by NDRAEY 2022. Carol Tea pixel-art by Ziyo Ling.\n");

    printf("Checking files...\n");
    for(int i=0; i<filescount; i++) {
        if(!vfs_exists(files[i])) {
            printf("%d. Eh... %s does not exist...\n", i, files[i]);
            return 1;
        }
    }
    for(int i=0; i<filescount; i++) {
        if(!vfs_exists(idle_files[i])) {
            printf("%d. Eh... %s does not exist...\n", i, idle_files[i]);
            return 1;
        }
    }

    printf("Loading SynapseOS Logo...\n");

    struct DukeImageMeta* smeta = duke_get_image_metadata("/initrd/res/SynapseOSLogo.duke");
    char* slogo = malloc(smeta->data_length);
    duke_get_image_data("/initrd/res/SynapseOSLogo.duke", *smeta, slogo);

    printf("Allocating first frames array...\n");

    Frames* frames_idle = David_new_frames_array(10);

    for(register int i=0; i<filescount; i++) {
        struct DukeImageMeta* meta = duke_get_image_metadata(idle_files[i]);
        
        char* origdata = malloc(meta->data_length);
        duke_get_image_data(idle_files[i], *meta, origdata);

        char* truedata = malloc(duke_calculate_bufsize(meta->width*3, meta->height*3, meta->alpha));
        duke_scale(origdata, meta->width, meta->height, meta->width*3, meta->height*3, meta->alpha, truedata);

        meta->width*=3;
        meta->height*=3;
        meta->data_length = meta->width*meta->height*(meta->alpha?4:3);

        free(origdata);

        David_set_frame(frames_idle, i, meta, truedata);
    }

    printf("Allocating second frames array...\n");
    Frames* frames = David_new_frames_array(10);
    
    for(register int i=0; i<filescount; i++) {
        struct DukeImageMeta* meta = duke_get_image_metadata(files[i]);
        
        char* origdata = malloc(meta->data_length);
        duke_get_image_data(files[i], *meta, origdata);

        char* truedata = malloc(duke_calculate_bufsize(meta->width*3, meta->height*3, meta->alpha));
        duke_scale(origdata, meta->width, meta->height, meta->width*3, meta->height*3, meta->alpha, truedata);

        meta->width*=3;
        meta->height*=3;
        meta->data_length = meta->width*meta->height*(meta->alpha?4:3);

        free(origdata);

        David_set_frame(frames, i, meta, truedata);
    }


    printf("Building animator...\n");
    Animator* animator_idle = David_new_animator(frames_idle);
    SynapseTTYInfo* ttyinfo = get_tty_info();

    int fbw = ttyinfo->width;
    int fbh = ttyinfo->height;

    printf("Width: %d\nHeight: %d\n", fbw, fbh);

    #define SHL 340

    // Script:
    //    IDLE - 1 time
    //    [wait] - 4 seconds
    //    Push out the SYnapseOS Logo - 1 cycle
    //    "What" text - end.

    int idleshft = frames_idle->origin[0]->imgmeta->height;

    // BUG: When Carol is "doing her idle pose", she's shifting by height incorrectly;
    // SOLUTION: Calculate max of all frame heights!
    for(int i=0; i<10; i++) {
        if(idleshft>frames_idle->origin[i]->imgmeta->height) {
            idleshft = frames_idle->origin[i]->imgmeta->height;
        }
    }

    for(int i=0; i<10; i++) {
        rect(fbw-SHL, 0, fbw-125, 360);
        David_do_draw_current_frame(animator_idle, ttyinfo->width-SHL, 
                                    (idleshft-frames_idle->origin[i]->imgmeta->height)+20);
        David_next_frame(animator_idle, 1);
        punch();
        sleep_ms(100);
    }

    sleep_ms(4000);

    printf("Freeing frames array <1>...\n");
    for(int i=0; i<filescount; i++) {
        free(frames_idle->origin[i]->imgdata);
    }

    free(animator_idle);

    Animator* animator = David_new_animator(frames); // Move here, coz #PF

    ttyinfo = get_tty_info();
    int hx = 0;

    while(1) {
        if(hx>375) break;
        rect(fbw-SHL, 0, fbw, 350);
        duke_rawdraw(slogo, smeta, (ttyinfo->width-100)+hx, 40);
        
        David_do_draw_current_frame(animator, (ttyinfo->width-SHL+10)+hx, 55);
        David_next_frame(animator, 1);
        punch();
        sleep_ms(100);

        hx+=15;
    }

    /* WARNING: Memory leakage of 400 KB!
    printf("Freeing frames array <2>...\n");
    for(int i=0; i<filescount; i++) {
        free(frames->origin[i]->imgdata);
    } */ // When trying to free allocated space, it throws an #PF exception!!!

    printf("Destroying animators...\n");

    printf("Animator 1 in: %d\n", animator);
    printf("Animator 1 in: %d\n", animator_idle);
    printf("FrameArray 1 in: %d\n", frames);
    printf("FrameArray 2 in: %d\n", frames_idle);
    //David_destroy_frames_array(frames); // It also causs#PF!
    //David_destroy_frames_array(frames_idle);

    printf("WARNING: The heap in SynapseOS 0.2.x is buggy... Carol will leave you ~400 kB of unfreeable memory...\n");

    return 0;
}