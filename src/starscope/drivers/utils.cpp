#include "starscope/drivers/clock.hpp"
#include "starscope/drivers/utils.hpp"
#include <string>
#include <chrono>

namespace Starscope::Utils
{

// template <typename T>
// std::string to_string(const T& value) {
//     std::ostringstream os;
//     os << value;
//     return os.str();
// }

StatefulSystem::StatefulSystem() : _is_ready(false) {
}

bool StatefulSystem::init() {
    if(!_is_ready)
        _is_ready = _init();

    return _is_ready;
}

void StatefulSystem::deinit() {
    if(_is_ready)
        _deinit();

    _is_ready = false;
}

bool StatefulSystem::ready() {
    return _is_ready;
}

bool StatefulSystem::_init() {
    return true;
}

void StatefulSystem::_deinit() {
}


Size::Size() : x(0), y(0) {
}
Size::Size(int x, int y) : x(x), y(y) {
}

std::string Size::to_string() const {
    return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
}
// std::ostream& operator<<(std::ostream& out, const Size& s) {
//     out << "[" << s.x << ", " << s.y << "]";
//     return out;
// }

Vector3::Vector3() : x(0.0), y(0.0), z(0.0) {
}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {
}

std::string Vector3::to_string() const {
    return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]";
}

// std::ostream& operator<<(std::ostream& out, const Vector3& v) {
//     out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
//     return out;
// }

Timer::Timer(const std::string msg, bool start_timer) :
_prefix(msg),
_duration(starscope_clock::duration::zero()) {
    if(start_timer)
        start();
};

starscope_clock::time_point Timer::start() {
    _begin = starscope_clock::now();
    return _begin;
}

starscope_clock::duration Timer::current() const {
    return starscope_clock::now() - _begin;
}

starscope_clock::duration Timer::interval() {
    starscope_clock::duration duration = current();
    start();
    return duration;
}

starscope_clock::duration Timer::end() {
    if(!end_set())
        _duration = current();
    return _duration;
}

starscope_clock::duration Timer::end_current() const {
    return _duration;
}

std::string Timer::prefix() const {
    return (_prefix.length() > 0 ? _prefix : "Duration");
}

bool Timer::end_set() const {
    return _duration != starscope_clock::duration::zero();
}

std::string Timer::to_string() const {
    starscope_clock::duration duration = end_set() ? end_current() : current();
    return prefix() + ": " + std::to_string(duration.count()) + "us\n";
}

// std::ostream& operator<<(std::ostream& out, const Timer& t) {
//     std::chrono::starscope_clock::duration duration = t.end_set() ? t.current() : t.end_current();
//     out << t.prefix() << ": " << duration;
//     return out;
// }

}

