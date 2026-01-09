/*******************************************************************************
 * Size: 18 px
 * Bpp: 2
 * Opts: --bpp 2 --size 18 --no-compress --stride 1 --align 1 --font iconfont.ttf --range 59116,58983,60248 --format lvgl -o symbol_statusbar.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef SYMBOL_STATUSBAR
#define SYMBOL_STATUSBAR 1
#endif

#if SYMBOL_STATUSBAR

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E667 "" */
    0x0, 0x1, 0x40, 0x0, 0x1, 0xbf, 0xfe, 0x40,
    0xb, 0x80, 0x2, 0xe0, 0x78, 0x0, 0x0, 0x2c,
    0x10, 0x7f, 0xfd, 0x4, 0x2, 0xd0, 0x7, 0x80,
    0x2, 0x0, 0x0, 0x80, 0x0, 0xb, 0xe0, 0x0,
    0x0, 0x38, 0x2c, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x2, 0x80, 0x0, 0x0, 0x3, 0xc0, 0x0,
    0x0, 0x1, 0x40, 0x0,

    /* U+E6EC "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1d, 0x0,
    0x0, 0x2f, 0xc0, 0x0, 0x6f, 0xf4, 0x0, 0x7f,
    0xff, 0x0, 0x7f, 0xff, 0xe0, 0x7f, 0xff, 0xfc,
    0x0, 0x0, 0x3f, 0x80, 0x0, 0x3, 0xf0, 0x0,
    0x0, 0x3e, 0x0, 0x0, 0x3, 0xd0, 0x0, 0x0,
    0x3c, 0x0, 0x0, 0x3, 0x40, 0x0, 0x0, 0x30,
    0x0,

    /* U+EB58 "" */
    0x1, 0x55, 0x55, 0x3, 0xff, 0xff, 0x1c, 0x0,
    0xb, 0x1c, 0x10, 0xb, 0xe0, 0x72, 0xcb, 0xe0,
    0x72, 0xcb, 0xe0, 0x71, 0xcb, 0xe0, 0x0, 0xb,
    0xe0, 0x0, 0xb, 0xe0, 0x0, 0xb, 0xe0, 0x0,
    0xb, 0xe0, 0x0, 0xb, 0xe0, 0x0, 0xb, 0xe0,
    0x0, 0xb, 0xfa, 0xaa, 0xaf, 0xff, 0xff, 0xff
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 288, .box_w = 16, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 52, .adv_w = 288, .box_w = 14, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 288, .box_w = 12, .box_h = 16, .ofs_x = 3, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x85, 0x4f1
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 58983, .range_length = 1266, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t symbol_statusbar = {
#else
lv_font_t symbol_statusbar = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if SYMBOL_STATUSBAR*/
