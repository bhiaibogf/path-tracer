//
// Created by bhiaibogf on 2022/2/23.
//

#include "timer.h"

Timer::Timer() {
    start_ = std::chrono::system_clock::now();
    end_ = std::chrono::system_clock::now();
}

void Timer::StartTimer(const std::string &start_msg) {
    std::cout << start_msg << "..." << std::endl;
    start_ = std::chrono::system_clock::now();
}

void Timer::StopTimer(const std::string &stop_msg) {
    end_ = std::chrono::system_clock::now();
    std::cout << stop_msg << ", " << GetTime() << " seconds used." << std::endl;
}

float Timer::GetTime() {
    return float(std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count()) / 1000.f;
}
