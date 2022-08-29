/**
 * @file Sovrum/main.c
 * @brief Простая демка
 * @author Андрей(Давид) Павленко (andrejpavlenko666@gmail.com)
 * @version 1.2
 * @date 26.07.2022
*/

#include <vesa.h>
#include "dukelib.h"
#include "font.h"
#include "../include/ctype.h"
#include "../include/time.h"
#include "../include/string.h"
#include "animator.h"
#include <stdio.h>

#define WIDTH 8
#define HEIGHT 12
#define SPACING 8

void draw_npx(int x, int y, int color, char size) {
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            draw_pixel(x+j, y+i, color);
        }
    }
}

void draw_nnpx(int x, int y, int color, char width, char height) {
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            draw_pixel(x+j, y+i, color);
        }
    }
}

void draw_by_matrix(uint8_t* table, uint8_t g, int32_t x, int32_t y, int color) {
    int32_t cx, cy;
    uint8_t mask[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    unsigned char *glyph = (uint8_t*)table + (g * 8);

    for (cy = 0; cy < 8; cy++) {
        for (cx = 0; cx < 8; cx++) {
            if (glyph[cy] & mask[cx]) {
                draw_nnpx(x + (cx*WIDTH), y + (cy*HEIGHT), color, WIDTH, HEIGHT);
            }
        }
    }
}

void draw_string(uint8_t* table, char* str, int32_t x, int32_t y, int color) {
    int tx = 0, ty = 0;

    int idx = 0;
    while(str[idx]) {
        char ch = str[idx];

        if(isdigit(ch)) {
            draw_by_matrix(font, ch-48, x+tx, y+ty, color);
            tx+=(WIDTH*8)+SPACING;
        }else if(ch==':') {
            draw_by_matrix(font, 10, x-16+tx, y+ty, color);
            tx+=(WIDTH*6);
        }
        idx++;
    }
}

void format_time(char* buf) {
    struct synapse_time* tm = get_time();
    
    int hours = tm->hours;
    int minutes = tm->minutes;
    int seconds = tm->seconds;

    char* hr = malloc(3), *mn = malloc(3), *sc = malloc(3);
    itoa(hours, hr); itoa(minutes, mn); itoa(seconds, sc);

    if(hours<=9) {
        buf = strcpy(buf, "0");
        buf = strcat(buf, hr);
    }else{
        buf = strcpy(buf, hr);
    }
    strcat(buf, ":");
    if(minutes<=9) {
        buf = strcat(buf, "0");
        buf = strcat(buf, mn);
    }else{
        buf = strcat(buf, mn);
    }
    strcat(buf, ":");
    if(seconds<=9) {
        buf = strcat(buf, "0");
        buf = strcat(buf, sc);
    }else{
        buf = strcat(buf, sc);
    }
    
    free(hr); free(mn); free(sc);
}

void format_time2(char* buf) {
    struct synapse_time* tm = get_time();
    
    int hours = tm->hours;
    int minutes = tm->minutes;
    int seconds = tm->seconds;

    char hr[3], mn[3], sc[3];
    itoa(hours, hr); itoa(minutes, mn); itoa(seconds, sc);

    
    if(hours<=9) {
        buf = strcpy(buf, "0");
        buf = strcat(buf, hr);
    }else{
        buf = strcpy(buf, hr);
    }
    strcat(buf, ":");
    
    if(minutes<=9) {
        buf = strcat(buf, "0");
        buf = strcat(buf, mn);
    }else{
        buf = strcat(buf, mn);
    }
    strcat(buf, ":");
    if(seconds<=9) {
        buf = strcat(buf, "0");
        buf = strcat(buf, sc);
    }else{
        buf = strcat(buf, sc);
    }
}

char* bg = "/initrd/res/Sovrum/background.duke";
char* carol_running[] = {
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

int main() {
    printf("Sovrum by NDRAEY (c)2022\n");

    SynapseTTYInfo* tty = get_tty_info();
    
    int fbsize = tty->width*tty->height*3;
    int fbwidth = tty->width;
    int fbheight = tty->height;
    printf("Screen width: %d\nScreen height: %d\n", fbwidth, fbheight);
    
    printf("Checking Carol Tea theme...");

    if(!vfs_exists(bg)) {
        print_str("[FAIL]\n");
        return 1;
    }else{
        print_str("[OKAY]\n");
    }

    printf("Loading background...\n");

    struct DukeImageMeta* bgmeta = duke_get_image_metadata(bg);
    char* _bgdata = malloc(bgmeta->data_length);
    duke_get_image_data(bg, *bgmeta, _bgdata);

    char* bgdata = malloc(duke_calculate_bufsize(tty->width, tty->height, bgmeta->alpha));
    duke_scale(_bgdata, bgmeta->width, bgmeta->height, tty->width, tty->height, bgmeta->alpha, bgdata);
    free(_bgdata);

    bgmeta->width = tty->width;
    bgmeta->height = tty->height;

    printf("Testing David...\n");
    printf("Allocating...\n");

    Frames* frames = David_new_frames_array(10);

    printf("Writing...\n");
    for(int i=0; i<10; i++) {
        struct DukeImageMeta* meta =  duke_get_image_metadata(carol_running[i]);
        char* imdata = malloc(meta->data_length);
        duke_get_image_data(carol_running[i], *meta, imdata);
        char* nimdata = malloc(duke_calculate_bufsize(meta->width*3, meta->height*3, meta->alpha));
        duke_scale(imdata, meta->width, meta->height, meta->width*3, meta->height*3, meta->alpha, nimdata);
        free(imdata);

        meta->width*=3;
        meta->height*=3;
        David_set_frame(frames, i, meta, nimdata);
    }

    printf("Getting ready for magic...\n");

    Animator* animator = David_new_animator(frames);
    int swidth = get_tty_info()->width;

    int32_t sx = -100;
    char tk[64] = { 0 };

    while(1) {
        if(getscancode()==16) break;
        
        if(sx >= swidth) { sx = -100; }
        duke_rawdraw(bgdata, bgmeta, 0, 0);
        
        memset(tk, 0, 64);
        format_time2(tk);
        draw_string(font, tk, 250, 90, 0xBBBBBB);

        David_do_draw_current_frame(animator, sx, 520);
        David_next_frame(animator, 1);
        sx += 45;
        
        punch();
        sleep_ms(25);
    } 

    //printf("Freeing...\n");

    /*for(int i=0; i<10; i++) {
        free(frames->origin[i]->imgmeta);
    }*/
    /*
    printf("Freeing (2)...\n");

    David_destroy_frames_array(frames);
    David_destroy_animator(animator);

    printf("OKAY!\n");
    */
    cleanScreen();
    
    return 0;
}