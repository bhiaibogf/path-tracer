//
// Created by bhiaibogf on 2022/2/23.
//

#include "timer.h"

Timer::Timer() {
    start_ = std::chrono::system_clock::now();
    end_ = std::chrono::system_clock::now();
}

void Timer::StartTimer() { start_ = std::chrono::system_clock::now(); }

void Timer::StopTimer() { end_ = std::chrono::system_clock::now(); }

float Timer::GetTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() / 1000.f;
}
