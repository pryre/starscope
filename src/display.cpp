#include <display.hpp>

#include <pcd8544_conf.h>
#include <pcd8544_port.h>
#include <pcd8544.h>
#define FALSE 0
#define TRUE 1


Display::Display(const Board &board) :
    _board(board) {
}

void Display::init() {

    pcd8544_reset();
    pcd8544_configure(&_config);
    pcd8544_set_mode(PCD8544_MODE_NORMAL);
    pcd8544_set_txt_language(PCD8544_LANG_RU);
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();
}

void Display::splash_screen(uint32_t display_ms) {
    pcd8544_setup_brush(FALSE, 2, 1);
    pcd8544_clear();
    pcd8544_set_cursor(0, 0);
    pcd8544_print_s("„“ŒŽ");
    pcd8544_set_cursor(0, 1);
    pcd8544_print_s("¯ã«ìâ");
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_set_cursor(10, 3);
    pcd8544_print_s("v2.0");
    pcd8544_setup_brush(TRUE, 1, 1);
    pcd8544_set_cursor(0, 5);
    pcd8544_print_s(" by pryre ");

    _board.sleep_ms(display_ms);

    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();
}

void Display::test_txt_size_and_color() {
    // font size 1, normal mode
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();
    pcd8544_set_cursor(0, 0);  // 0, 0 for real coords
    pcd8544_print_s("txt 1");

    // font size 2, inverse mode
    pcd8544_setup_brush(TRUE, 2, 1);
    pcd8544_set_cursor(1, 1);  // 0, 16 for real coords
    pcd8544_print_s("txt 2");
    _delay_ms(2000);

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

void Display::test_txt_lang_and_frame() {
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();

    // text with english and russian words printed in a frame 72x36 px
    pcd8544_set_cursor(1, 1);
    pcd8544_print_s_f(6, 78, 4, "In Russian, word bear means word ");
    pcd8544_set_txt_language(PCD8544_LANG_RU);
    pcd8544_print_s_f(6, 78, 4, "\"медведь\".");
    pcd8544_set_txt_language(PCD8544_LANG_EN);
}

void Display::test_img_scale() {
    /*
     * Here we are going to print a simple logo on the whole display, but the
     * logo is only 14 (instead of 504) bytes size.
     */
    pcd8544_setup_brush(FALSE, 1, 6);
    pcd8544_clear();
    pcd8544_draw_img(0, 0, &_simple_logo_image);
}
