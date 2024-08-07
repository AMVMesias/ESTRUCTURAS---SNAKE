#include "engine/timer/Timer.h"

Timer::Timer(float starting_seconds) 
    : starting_seconds_(starting_seconds)
    , elapsed_seconds_(starting_seconds) {}


void Timer::Update(float dt) {
    elapsed_seconds_ += dt;
}

float Timer::GetElapsedSeconds() const {
    return elapsed_seconds_;
}
