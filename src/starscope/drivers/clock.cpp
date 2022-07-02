#include "starscope/drivers/clock.hpp"
#include "pico/stdlib.h"

namespace Starscope
{
    starscope_clock::time_point starscope_clock::now() noexcept {
        return starscope_clock::time_point{starscope_clock::duration{time_us_64()}};
    }
}
