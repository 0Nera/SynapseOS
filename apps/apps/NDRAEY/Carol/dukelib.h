/**
 * @file imaging.h
 * @brief Описания функций imaging.h
 * @author Андрей(Давид) Павленко (andrejpavlenko666@gmail.com)
 */

#ifndef IMAGING_H
#define IMAGING_H

struct DukeImageMeta {
    short width;
    short height;
    int data_length;
    char alpha;
};

char duke_draw_from_file(char *filename, int sx, int sy);
struct DukeImageMeta* duke_get_image_metadata(char *filename);
void duke_get_image_data(char* filename, struct DukeImageMeta meta, char* out);
unsigned int duke_calculate_bufsize(unsigned int width, unsigned int height, unsigned int alpha);
void duke_rawdraw(char* data, struct DukeImageMeta* meta, int sx, int sy);
void duke_scale(char* pixels, unsigned int w1, unsigned int h1, int w2, int h2, char alpha, char* out);
char duke_draw_scaled(char* filename, int width, int height, int x, int y);

#endif
