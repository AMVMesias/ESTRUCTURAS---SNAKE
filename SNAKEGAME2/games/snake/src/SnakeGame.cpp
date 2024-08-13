#include "snake/include/SnakeGame.h"

#include "engine/Rectangle.h"

SnakeGame::SnakeGame() 
    : engine_("Snake", 800, 600)
    , snake_(engine_)
    , food_factory_(engine_, snake_)
    , food_(food_factory_.CreateFood())
    , boundaries_{{
        {engine_, snake_, {0.f, 0.f, engine_.GetFWindowWidth(), 1.f}}, // TOP 
        {engine_, snake_, {0.f, 0.f, 1.f, engine_.GetFWindowHeight()}}, // LEFT
        {engine_, snake_, {engine_.GetFWindowWidth() - 1.f, 0.f, 1.f, engine_.GetFWindowHeight()}}, // RIGHT
        {engine_, snake_, {0.f, engine_.GetFWindowHeight() - 1.f, engine_.GetFWindowWidth(), 1.f}} // BOTTOM
    }} {}

void SnakeGame::OnMouseEvent(EMouseEventType event_type, int x, int y) {
    return;
}

void SnakeGame::OnKeyboardEvent(EKeyEventType event_type, SDL_Scancode scancode) {
    if (!snake_.IsAlive()) {
        if (event_type == EKeyEventType::KEY_DOWN && scancode == SDL_SCANCODE_SPACE) {
            snake_.Reset();
        } 
    }
    snake_.OnKeyboardEvent(event_type, scancode);
}

void SnakeGame::Start() {
    engine_.Run(*this);
}

void SnakeGame::Update(float dt) {
    if (food_ && food_->IsMarkedForDestroy()) {
        food_ = food_factory_.CreateFood();
    }

    if (snake_.IsAlive()) {
        snake_.Update(dt);
    }
}

void SnakeGame::Render() {
    snake_.Render();
    
    for (auto& boundary : boundaries_) {
        boundary.Render();
    }

    if (food_) {
        food_->Render();
    }
}

