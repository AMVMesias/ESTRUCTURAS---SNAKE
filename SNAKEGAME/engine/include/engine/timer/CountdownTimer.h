#pragma once

#include "engine/timer/Timer.h"

class CountdownTimer : public Timer {
public:
    CountdownTimer(float seconds_to_finish, float starting_seconds = 0.f, bool is_looping = true);

    // Timer
    void Update(float dt) override;

    bool DidFinish() const;
    void Reset();
    void AddSecondsToFinish(float seconds);
    void SubtractSecondsToFinish(float seconds);
    void SetSecondsToFinish(float seconds_to_finish);
    float GetSecondsToFinish() const;

private:
    float seconds_to_finish_;
    bool did_finish_;
    bool is_looping_;
};
