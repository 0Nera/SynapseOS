#ifndef IMAGING_H
#define IMAGING_H

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