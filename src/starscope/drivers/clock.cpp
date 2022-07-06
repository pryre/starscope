#include "starscope/drivers/clock.hpp"
#include "starscope/hardware_abstraction.hpp"

namespace Starscope
{
    starscope_clock::time_point starscope_clock::now() noexcept {
        return starscope_clock::time_point{starscope_clock::duration{time_monotonic_us()}};
    }
}
