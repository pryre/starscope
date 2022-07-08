#include "starscope/user_interface/heartbeat.hpp"
#include "starscope/hardware_abstraction.hpp"

using namespace Starscope;
namespace Starscope::UserInterface::Heartbeat {

Light::Light() :
_blink_count(0),
_goal_count(0),
_flag_reset(true) {
}

bool Light::_init() {
    _flag_reset = true;

    heartbeat_set(false);

    return true;
}

void Light::_deinit() {
    heartbeat_set(false);
}


void Light::_update(const starscope_clock::time_point now) {
    //We split the steps up into blocks of BLINK_PERIOD
    //starting at the start of a new HEARTBEAT_PERIOD
    //This means that we get a max of BLINK_PERIOD/2 blinks
    //to work with for our number of blocks per mode

    const auto heartbeat_progress = now.time_since_epoch() % HEARTBEAT_PERIOD;
    const unsigned int current_step = heartbeat_progress / STEP_PERIOD;
    const auto step_progress = heartbeat_progress % STEP_PERIOD;

    //Flag a reset if we've progressed past the first step
    if(current_step > 0) {
        _flag_reset = true;
    } else if(_flag_reset) {
        //Reset state if we're on the first step and if we haven't just reset
        //_goal count is a maximum index for _blink_count, so -1 from the number of blinks
        _goal_count = _get_num_steps() - 1;
        _blink_count = 0;
        _flag_reset = false;
    }

    bool led_on = false;

    //Check to see if we should be blinking
    if (_blink_count == current_step) {
        if(step_progress < BLINK_PERIOD) {
            //If we're on a 'blink on' part of the step
            led_on = true;
        } else if(_blink_count < _goal_count) {
            //If we're on a 'blink off' part of the step
            //so tick up our counter if our index is less than the max index
            _blink_count++;
        }
    }

    heartbeat_set(led_on);
}

unsigned int Light::_get_num_steps() {
    unsigned int num_steps = 0;

    switch (System::get_mode())
    {
    case System::Mode::INITIALIZE: {
        num_steps = BLINKS_INITIALIZE;
        break;
    }
    case System::Mode::STANDBY : {
        num_steps = BLINKS_STANDBY;
        break;
    }
    case System::Mode::ACTIVE : {
        num_steps = BLINKS_ACTIVE;
        break;
    }
    case System::Mode::SLEEP : {
        num_steps = BLINKS_SLEEP;
        break;
    }
    case System::Mode::SHUTDOWN : {
        num_steps = BLINKS_SHUTDOWN;
        break;
    }
    }

    return num_steps;
}

}
