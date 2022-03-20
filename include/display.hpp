#ifndef _STARSCOPE_DISPLAY_H
#define _STARSCOPE_DISPLAY_H

#include <pcd8544.h>
#include <board.hpp>

class Display {
    private:
        const Board _board;
        const pcd8544_config _config = {
            .bias = 0,
            .contrast = 0x3F,
            .tempcoeff = 0
        };

        const uint8_t _simple_logo_bitmap[14] = {
            0xFF, 0x81, 0xC3, 0xA5, 0x99, 0xA5, 0xDB,
            0xDB, 0xA5, 0x99, 0xA5, 0xC3, 0x81, 0xFF
        };

        const pcd8544_image _simple_logo_image = {
            .bitmap = (const uint8_t *)_simple_logo_bitmap,
            .lookup = 0,    //FALSE
            .width_px = 14,
            .height_pg = 1
        };

    public:
        Display(const Board &board);

        // Initialize the GPIO pins
        void init();

        // Show a splash screen for a set time
        void splash_screen(uint32_t display_ms);

        // Test function for different size and colour text
        void test_txt_size_and_color();

        // Test function for different languages and framing
        void test_txt_lang_and_frame();

        // Test function for image logo scaling
        void test_img_scale();
};

#endif