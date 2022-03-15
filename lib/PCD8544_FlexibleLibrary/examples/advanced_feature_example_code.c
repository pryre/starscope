/*
 * This is just an example code for some non-trivial features of this library.
 *
 * delay_ms(ms)     - millisecond delay function;
 */

#include <pcd8544_conf.h>
#include <pcd8544.h>

#include <inttypes.h>
#define FALSE 0
#define TRUE 1

static const uint8_t simple_logo_bitmap[14] = {
     0xFF, 0x81, 0xC3, 0xA5, 0x99, 0xA5, 0xDB,
     0xDB, 0xA5, 0x99, 0xA5, 0xC3, 0x81, 0xFF
};

static const pcd8544_image simple_logo_image = {
    .bitmap = (const uint8_t *)simple_logo_bitmap,
    .lookup = FALSE,
    .width_px = 14,
    .height_pg = 1
};

void test_txt_size_and_color(void)
{
    // font size 1, normal mode
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();
    pcd8544_set_cursor(0, 0);  // 0, 0 for real coords
    pcd8544_print_s("txt 1");

    // font size 2, inverse mode
    pcd8544_setup_brush(TRUE, 2, 1);
    pcd8544_set_cursor(1, 1);  // 0, 16 for real coords
    pcd8544_print_s("txt 2");
    delay_ms(2000);

    // font size 3, normal mode
    pcd8544_setup_brush(FALSE, 3, 1);
    /*
     * if called befor pcd8544_setup_brush() the display will be cleared with
     * black color
     */
    pcd8544_clear();
    pcd8544_set_cursor(1, 0);  // 18, 0 for real coords
    pcd8544_print_s("txt 3");
}

void test_txt_lang_and_frame(void)
{
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();

    // text with english and russian words printed in a frame 72x36 px
    pcd8544_set_cursor(1, 1);
    pcd8544_print_s_f(6, 78, 4, "In Russian, word bear means word ");
    pcd8544_set_txt_language(PCD8544_LANG_RU);
    pcd8544_print_s_f(6, 78, 4, "\"медведь\".");
    pcd8544_set_txt_language(PCD8544_LANG_EN);
}

void test_img_scale(void)
{
    /*
     * Here we are going to print a simple logo on the whole display, but the
     * logo is only 14 (instead of 504) bytes size.
     */
    pcd8544_setup_brush(FALSE, 1, 6);
    pcd8544_clear();
    pcd8544_draw_img(0, 0, &simple_logo_image);
}