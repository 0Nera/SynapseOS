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
    int   data_length;
    char  alpha;
};

// Returns 0 if OK, 1 if ERR
char draw_from_file(char *filename, int sx, int sy);
struct DukeImageMeta* get_image_metadata(char *filename);

#endif
