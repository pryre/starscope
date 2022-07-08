#ifndef _STARSCOPE_USER_INTERFACE_HEARTBEAT_H
#define _STARSCOPE_USER_INTERFACE_HEARTBEAT_H

#include "starscope/drivers/utils.hpp"
#include "starscope/system/state.hpp"
#include "starscope/drivers/clock.hpp"
#include <chrono>

using namespace std::chrono_literals;
using namespace Starscope;
namespace Starscope::UserInterface::Heartbeat {

//How often the heartbeat blinks run
const starscope_clock::duration HEARTBEAT_PERIOD = 2s;

//How long the each heartbeat blink is active for
const starscope_clock::duration BLINK_PERIOD = 100ms;

//How long the between the start of each heartbeat blink
const starscope_clock::duration STEP_PERIOD = 200ms;

const unsigned int BLINKS_INITIALIZE = 10;
const unsigned int BLINKS_STANDBY = 1;
const unsigned int BLINKS_ACTIVE = 2;
const unsigned int BLINKS_SLEEP = 0;
const unsigned int BLINKS_SHUTDOWN = 5;

class Light : public Utils::StatefulSystem {
    private:
    unsigned int _blink_count;
    unsigned int _goal_count;
    bool _flag_reset;

    public:
    Light();

    private:
    bool _init();
    void _deinit();
    void _update(const starscope_clock::time_point now);

    unsigned int _get_num_steps();
};

}

#endif
