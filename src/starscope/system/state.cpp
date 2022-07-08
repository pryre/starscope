#include "starscope/system/common.hpp"
#include "starscope/system/state.hpp"

using namespace Starscope;

//Hidden "anonymous namespace" to house our global variables
namespace {
    System::Mode _current_mode = System::Mode::INITIALIZE;
}

System::Mode System::get_mode() {
    return _current_mode;
}

bool System::set_mode(const System::Mode mode) {
    _current_mode = mode;
    return true;
}

