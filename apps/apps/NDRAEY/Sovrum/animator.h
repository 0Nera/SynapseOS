#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include "dukelib.h"

typedef struct DavidTheAnimator_FrameEntry {
    char* imgdata;
    struct DukeImageMeta* imgmeta;
} FrameEntry;

typedef struct DavidTheAnimator_Frames {
    int length;
    FrameEntry** origin;
} Frames;

typedef struct DavidTheAnimator {
    Frames* frames;
    int length;
    int index;
} Animator;

Frames* David_new_frames_array(unsigned int length);
void David_destroy_frames_array(Frames* frames);
void David_set_frame(Frames* frames, int idx, struct DukeImageMeta* imgmeta, char* imgdata);
Animator* David_new_animator(Frames* frames);
void David_destroy_animator(Animator* animator);
void David_do_draw_current_frame(Animator* anim, int x, int y);
void David_next_frame(Animator* anim, char loop);

#endif