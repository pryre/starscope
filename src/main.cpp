#include <board.hpp>
#include <display.hpp>

int main()
{
    Board board;
    Display display(board);

	board.gpio_init();
	board.spi_init();

    display.init();
    display.splash_screen(3000);
	display.test_img_scale();

    board.blink_forever(2);

	return 0;
}
