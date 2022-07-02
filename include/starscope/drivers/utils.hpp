#ifndef _STARSCOPE_DRIVERS_UTILS_H
#define _STARSCOPE_DRIVERS_UTILS_H

#include <string>
#include <chrono>
#include "starscope/drivers/clock.hpp"

namespace Starscope::Utils
{

class StatefulSystem {
    private:
    bool _is_ready;

    public:
    StatefulSystem();
    bool init();
    void deinit();
    bool ready();

    private:
    virtual bool _init();
    virtual void _deinit();
};


class Size {
    public:
    int x;
    int y;

    public:
    Size();
    Size(int x, int y);

    std::string to_string() const;
    // friend std::ostream& operator<<(std::ostream& out, const Size& s);
};

class Vector3 {
    public:
    float x;
    float y;
    float z;

    public:
    Vector3();
    Vector3(float x, float y, float z);

    std::string to_string() const;
    // friend std::ostream& operator<<(std::ostream& out, const Vector3& t);
};


class Timer {
    private:
    std::string _prefix;
    starscope_clock::time_point _begin;
    starscope_clock::duration _duration;

    public:
    Timer(const std::string msg, bool start_timer = true);
    //Start the Timer tic
    starscope_clock::time_point start();
    //Get the current duration since the Timer tic
    starscope_clock::duration current() const;
    //Get the current duration since the Timer tic and start a new tic
    starscope_clock::duration interval();
    //Get the current duration since the Timer tic and mark for future output
    starscope_clock::duration end();
    //Get marked end duration
    starscope_clock::duration end_current() const;
    //Get prefix string
    std::string prefix() const;

    std::string to_string() const;
    // friend std::ostream& operator<<(std::ostream& out, const Timer& t);

    private:
    bool end_set() const;
};

}

#endif //_STARSCOPE_DRIVERS_UTILS_H
