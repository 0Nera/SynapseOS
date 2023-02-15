/**
 * @file psf.h
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief 
 * @version 0.1.0
 * @date 02-12-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 (c) 2022
 * 
 */

#include <libk.h>


#ifndef _PSF_DRAW_H
#define _PSF_DRAW_H 1

#define PSF_V1 0x01
#define PSF_V2 0x02

#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04
#define PSF1_MAXMODE    0x05

#define PSF1_SEPARATOR  0xFFFF
#define PSF1_STARTSEQ   0xFFFE


#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct __attribute__((packed)) {
    uint8_t magic[2];
    uint8_t mode;      
    uint8_t charheight;
} psf_t;


#define PSF2_MAGIC0     0x72
#define PSF2_MAGIC1     0xb5
#define PSF2_MAGIC2     0x4a
#define PSF2_MAGIC3     0x86

#define PSF2_MAGIC 0x864ab572 

#define PSF2_HAS_UNICODE_TABLE 0x01

#define PSF2_MAXVERSION 0

#define PSF2_SEPARATOR  0xFF
#define PSF2_STARTSEQ   0xFE

typedef struct {
    uint8_t magic[4];
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} psf2_t;

//extern psf_t *_binary_files_fonts_SynapseOS_UniCyr_8x16_psf_start;
//extern psf2_t *_binary_font_psf2_psf_start;
extern psf_t *_binary_kernel_src_graf_font_psf_start;


#endif // psf.h