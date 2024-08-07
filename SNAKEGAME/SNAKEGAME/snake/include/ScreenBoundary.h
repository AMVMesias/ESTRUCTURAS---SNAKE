#pragma once

#include "engine/Engine.h"
#include "engine/collision/ICollidable.h"

#include "Snake.h"

class ScreenBoundary : public ICollidable {
public:
    ScreenBoundary(Engine& engine, Snake& snake, Rectangle rect);
    ~ScreenBoundary();

    void Render();

    // ICollidable
    virtual const Rectangle& GetBoundingBox() const override;
    virtual void OnCollision(ICollidable& other) override;
    virtual unsigned int GetLayer() const override;
    virtual unsigned int GetMask() const override;

private:
    Engine& engine_;
    Snake& snake_;
    Rectangle rect_;
};
