/**
 * @file imaging.h
 * @brief Описания функций imaging.h
 * @author Андрей(Давид) Павленко (andrejpavlenko666@gmail.com)
 */

#ifndef IMAGING_H
#define IMAGING_H

/**
 * @brief Структура изображения Duke.
 * @param width - ширина
 * @param height - высота
 * @param data_length - длина всех "сырых" пикселей
 * @param alpha - указывает на наличие альфа слоя
 */

struct DukeImageMeta {
    short width;
    short height;
    int data_length;
    char alpha;
};

// Returns 0 if OK, 1 if ERR
char duke_draw_from_file(char *filename, int sx, int sy);
struct DukeImageMeta* duke_get_image_metadata(char *filename);
void duke_get_image_data(char* filename, struct DukeImageMeta meta, char* out);
unsigned int duke_calculate_bufsize(unsigned int width, unsigned int height, unsigned int alpha);
void duke_rawdraw(char* data, struct DukeImageMeta* meta, int sx, int sy);
void duke_scale(char* pixels, unsigned int w1, unsigned int h1, int w2, int h2, char alpha, char* out);
char duke_draw_scaled(char* filename, int width, int height, int x, int y);

#endif
