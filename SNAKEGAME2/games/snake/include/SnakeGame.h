#pragma once

#include "engine/IGame.h"
#include "engine/Engine.h"
#include "engine/timer/CountdownTimer.h"

#include "Snake.h"
#include "FoodFactory.h"
#include "Food.h"
#include "ScreenBoundary.h"

#include <memory>
#include <array>

class SnakeGame : public IGame {
public:
    SnakeGame();

    // IGame
    void Start() override;
    void Update(float dt) override;
    void Render() override;

    // IMouseEventsListener
    void OnMouseEvent(EMouseEventType event_type, int x, int y) override;

    // IKeyboardEventsListener
    void OnKeyboardEvent(EKeyEventType event_type, SDL_Scancode scancode) override;

private:
    Engine engine_;
    Snake snake_;
    FoodFactory food_factory_;
    std::unique_ptr<Food> food_;
    std::array<ScreenBoundary, 4> boundaries_;
};
