/**
 * @file imaging.c
 * @brief Встраиваемая библиотека рисования изображений формата Duke
 * @author Андрей(Давид) Павленко (andrejpavlenko666@gmail.com)
 * @version 1.2
 * @date 26.07.2022
*/

#include <io/imaging.h>
#include <kernel.h>

/**
 * @brief Получает метаданные изображения Duke.
 * @param filename - Имя файла
 * @return Структуру с метаданными при успехе
 * @return 0 При ошибке
 */

struct DukeImageMeta* duke_get_image_metadata(char *filename) {
    if(vfs_exists(filename)) {
        char* rmeta = kheap_malloc(9);
        vfs_read(filename, 0, 9, rmeta);
        return (struct DukeImageMeta*)rmeta;
    }
    return 0;
}

void duke_get_image_data(char* filename, struct DukeImageMeta meta, char* out) {
    if(vfs_exists(filename)) {
        vfs_read(filename, 9, meta.data_length, out);
    }
}

/**
 * @brief Функция отрисовки изображения
 * @param filename - Имя файла
 * @param sx - Координата x
 * @param sy - Координата y
 * @return 0 - OK
 * @return 1 - ERROR
 */
char duke_draw_from_file(char *filename, int sx, int sy) {
    char meta[9];
    if(vfs_exists(filename)) {
        vfs_read(filename, 0, 9, meta);
        struct DukeImageMeta* realmeta = (struct DukeImageMeta*)meta;

        qemu_log("Width: %d\nHeight: %d\nAlpha: %d\n", realmeta->width, realmeta->height, realmeta->alpha);

        qemu_log("Allocating %d bytes for image", realmeta->data_length); // Это тоже пофиксило падение, но ПОЧЕМУ??? (llvm-10)
        char *imagedata = kheap_malloc(realmeta->data_length);
        
        vfs_read(filename, 9, realmeta->data_length, imagedata);

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
        qemu_log("Freeing memory...");
    }else{ return 1; }
    return 0;
}

void duke_rawdraw(char* data, struct DukeImageMeta* meta, int sx, int sy){
    int x = 0, y = 0;
    char mod = meta->alpha?4:3;
    while(y<meta->height) {
        x = 0;
        while(x<meta->width) {
            int px = pixidx(meta->width*mod, x*mod, y);
            int r = data[px];
            int g = data[px+1];
            int b = data[px+2];
            int a = data[px+3];
            int color = ((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff);
            if(mod && a!=0) {
                set_pixel(sx+x, sy+y, color);
            }
            x++;
        }
        y++;
    }
}

void duke_scale(char* pixels, unsigned int w1, unsigned int h1, int w2, int h2, char alpha, char* out) {
    int scr_w = (w1<<16)/w2;
    int scr_h = (h1<<16)/h2;

    int x = 0;
    int y = 0;

    int x2 = 0;
    int y2 = 0;

    char mod = alpha?4:3;
    while(y<h2) {
        x = 0;
        while(x<w2) {
            x2 = (x*scr_w)>>16;
            y2 = (y*scr_h)>>16;

            out[pixidx(w2*mod, x*mod, y)] = pixels[pixidx(w1*mod, x2*mod, y2)];
            out[pixidx(w2*mod, x*mod, y)+1] = pixels[pixidx(w1*mod, x2*mod, y2)+1];
            out[pixidx(w2*mod, x*mod, y)+2] = pixels[pixidx(w1*mod, x2*mod, y2)+2];
            if(alpha) {
                out[pixidx(w2*mod, x*mod, y)+3] = pixels[pixidx(w1*mod, x2*mod, y2)+3];
            }
            x++;
        }
        y++;
    }
}

/**
 * @brief Функция вычисления позиции по координатам
 * @param filename - Имя файла
 * @param sx - Координата x
 * @param sy - Координата y
 * @return 0 - OK, 1 - ERROR
 */
int pixidx(int width, int x, int y) {
    return width*y + x;
}

unsigned int duke_calculate_bufsize(unsigned int width, unsigned int height, unsigned int alpha) {
    return width*height*(alpha?4:3);
}


/*
DUKE SCALING EXAMPLE:

    struct DukeImageMeta* frw = duke_get_image_metadata("/initrd/res/SynapseOSLogo.duke");
    if(frw==0) {tty_printf("Return 0!\n");}

    unsigned int bsize = duke_calculate_bufsize(frw->width, frw->height, frw->alpha);

    char* imdata = kheap_malloc(bsize);
    duke_get_image_data("/initrd/res/SynapseOSLogo.duke", *frw, imdata);

    unsigned int nsize = duke_calculate_bufsize(300, 300, 1);
    char *ndata = kheap_malloc(nsize);

    duke_scale(imdata, frw->width, frw->height, 300, 300, 1, ndata);

    kheap_free(imdata);
    frw->width = 300;
    frw->height= 300;
    duke_rawdraw(ndata, frw, getWidthScreen()-300, 0);
    kheap_free(ndata);
*/

char duke_draw_scaled(char* filename, int width, int height, int x, int y) {
    if(!vfs_exists(filename)) return 1;
    struct DukeImageMeta* origimg = duke_get_image_metadata(filename);
    if(origimg==0) return 1;

    unsigned int bsize = duke_calculate_bufsize(origimg->width, origimg->height, origimg->alpha);

    char* imdata = kheap_malloc(bsize);
    duke_get_image_data(filename, *origimg, imdata);

    unsigned int nsize = duke_calculate_bufsize(width, height, origimg->alpha);
    char *ndata = kheap_malloc(nsize);

    duke_scale(imdata, origimg->width, origimg->height, width, height, origimg->alpha, ndata);
    kheap_free(imdata);

    origimg->width = width;
    origimg->height= height;
    duke_rawdraw(ndata, origimg, x, y);
    kheap_free(ndata);
    return 0;
}