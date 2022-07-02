#ifndef _STARSCOPE_DRIVERS_CLOCK_H
#define _STARSCOPE_DRIVERS_CLOCK_H

#include <chrono>

namespace Starscope
{

// uint64_t time_us_64() {
//     return 0;
// }

struct starscope_clock {
    using period     = std::ratio<1, 1'000'000>;
    using rep        = uint64_t;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<starscope_clock>;
    static constexpr bool is_steady = true;

    static time_point now() noexcept;
};

}
// using starscope_clock = std::chrono::steady_clock

#endif //_STARSCOPE_DRIVERS_CLOCK_H
