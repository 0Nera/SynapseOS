#include <stdlib.h>
#include "animator.h"
#include "dukelib.h"

Frames* David_new_frames_array(unsigned int length) {
    Frames* frames = malloc(sizeof(Frames*));  

    frames->origin = (FrameEntry**)malloc(sizeof(FrameEntry**)*length);
    frames->length = length;

    for(int i=0; i<length; i++) {
        frames->origin[i] = malloc(sizeof(FrameEntry));
    }

    return frames;
}

void David_destroy_frames_array(Frames* frames) {
    for(int i=0; i<frames->length; i++) {
        free(frames->origin[i]);
    }
    free(frames->origin);
    free(frames);
}

void David_set_frame(Frames* frames, int idx, struct DukeImageMeta* imgmeta, char* imgdata) {
    frames->origin[idx]->imgdata = imgdata;
    frames->origin[idx]->imgmeta = imgmeta;
}

Animator* David_new_animator(Frames* frames) {
    Animator* animator = malloc(sizeof(Animator*));

    animator->frames = frames;
    animator->length = frames->length;
    animator->index = 0;

    return animator;
}

void David_destroy_animator(Animator* animator) {
    if(animator) free(animator);
}

void David_do_draw_current_frame(Animator* anim, int x, int y) {
    duke_rawdraw(anim->frames->origin[anim->index]->imgdata, 
                 anim->frames->origin[anim->index]->imgmeta, x, y);
}

void David_next_frame(Animator* anim, char loop) {
    //qemu_printf("DAVID: I: %d, L: %d, WW: %d, WH: %d\n", anim->index, anim->length,
    //            anim->frames->origin[anim->index]->imgmeta->width,
    //            anim->frames->origin[anim->index]->imgmeta->height
    //            );
    if(loop) {
        if(anim->index>=anim->length-1) { // I'll fix it later
            anim->index = 0;
            return;
        }
    }else{
        if(anim->index>=anim->length) {
            anim->index = anim->length-1;
            return;
        }
    }
    anim->index++;
}