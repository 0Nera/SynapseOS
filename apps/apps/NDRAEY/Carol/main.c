#include <stdio.h>
#include <vesa.h>
#include "animator.h"
#include "dukelib.h"
#include <ports.h>

char* files[] = {
    "/initrd/res/Carol/run/carol_running_0.png.duke",
    "/initrd/res/Carol/run/carol_running_1.png.duke",
    "/initrd/res/Carol/run/carol_running_2.png.duke",
    "/initrd/res/Carol/run/carol_running_3.png.duke",
    "/initrd/res/Carol/run/carol_running_4.png.duke",
    "/initrd/res/Carol/run/carol_running_5.png.duke",
    "/initrd/res/Carol/run/carol_running_6.png.duke",
    "/initrd/res/Carol/run/carol_running_7.png.duke",
    "/initrd/res/Carol/run/carol_running_8.png.duke",
    "/initrd/res/Carol/run/carol_running_9.png.duke"
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

    printf("Allocating frames...\n");
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
    Animator* animator = David_new_animator(frames);
    SynapseTTYInfo* ttyinfo = get_tty_info();

    int fbw = ttyinfo->width;
    int fbh = ttyinfo->height;

    printf("Width: %d\nHeight: %d\n", fbw, fbh);

    while(1) {
        rect(fbw-200, 120, fbw, 360);
        David_do_draw_current_frame(animator, ttyinfo->width-200, 120);
        David_next_frame(animator, 1);
        punch();
        sleep_ms(110);
    }

    for(register int i=0; i<filescount; i++) {
        free(frames->origin[i]->imgdata);
    }

    David_destroy_frames_array(frames);

    return 0;
}