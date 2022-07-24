// Image Drawing library by Andrey(David) Pavlenko aka NDRAEY ~2022~

#include <drivers/vfs.h>
#include <mem/kheap.h>
//#include <io/tty.h>

struct ImageMeta {
    short width;
    short height;
    int   data_length;
    char  alpha;
};

// Returns 0 if OK, 1 if ERR
char draw_from_file(char *filename, int sx, int sy) {
    char meta[4];
    if(vfs_exists(filename)) {
        vfs_read(filename, 0, 9, meta);
        struct ImageMeta* realmeta = (struct ImageMeta*)meta;

        char *imagedata = kheap_malloc(realmeta->data_length);
        
        vfs_read(filename, 8, realmeta->data_length, imagedata);

        int x = 0, y = 0;
        char mod = realmeta->alpha?4:3;
        while(y<realmeta->height) {
            x = 0;
            while(x<realmeta->width) {
                int px = pixidx(realmeta->width*mod, x*mod, y);
                int r = imagedata[px];
                int g = imagedata[px+1];
                int b = imagedata[px+2];
                int a = imagedata[px+3];
                int color = ((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff);
                if(mod && a!=0) {
                    set_pixel(sx+x, sy+y, color);
                }
                x++;
            }
            y++;
        }

        kheap_free(imagedata);
    }else{ return 1; }
    return 0;
}

int pixidx(int width, int x, int y) {
    return width*y + x;
}