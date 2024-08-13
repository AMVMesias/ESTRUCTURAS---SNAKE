#pragma once

class Timer {
public:
    Timer(float starting_seconds = 0.f);
    virtual ~Timer() = default;

    virtual void Update(float dt);
    float GetElapsedSeconds() const;

protected:
    const float starting_seconds_;
    float elapsed_seconds_;
};
