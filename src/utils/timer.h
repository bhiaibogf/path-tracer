//
// Created by bhiaibogf on 2022/2/23.
//

#ifndef PATH_TRACER_TIMER_H
#define PATH_TRACER_TIMER_H


#include <chrono>
#include <iostream>

class Timer {
public:
    Timer();

    ~Timer() = default;

    void StartTimer(const std::string &start_msg);

    void StopTimer(const std::string &stop_msg);

    float GetTime();

private:
    std::chrono::time_point<std::chrono::system_clock> start_, end_;

};


#endif //PATH_TRACER_TIMER_H
