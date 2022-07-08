#ifndef _STARSCOPE_SYSTEM_COMMON_H
#define _STARSCOPE_SYSTEM_COMMON_H

namespace Starscope::System {

typedef enum {
    INITIALIZE,
    STANDBY,
    ACTIVE,
    SLEEP,
    SHUTDOWN,
} Mode;

}

#endif
