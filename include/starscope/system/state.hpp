#ifndef _STARSCOPE_SYSTEM_STATE_H
#define _STARSCOPE_SYSTEM_STATE_H

#include "starscope/system/common.hpp"

using namespace Starscope;
namespace Starscope::System {

Mode get_mode();
bool set_mode(const Mode mode);

}

#endif
