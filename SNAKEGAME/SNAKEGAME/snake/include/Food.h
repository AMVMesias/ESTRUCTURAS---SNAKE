#pragma once

#include "engine/Engine.h"
#include "engine/Rectangle.h"
#include "engine/collision/ICollidable.h"

class Food : public ICollidable {
public:
    Food(Engine& engine, Rectangle rect);
    ~Food();

    void Render();
    bool IsMarkedForDestroy() const;

    // ICollidable
    const Rectangle& GetBoundingBox() const override;
    void OnCollision(ICollidable& other) override;
    unsigned int GetLayer() const override;
    unsigned int GetMask() const override;

private:
    Engine& engine_;
    Rectangle rect_;
    bool is_marked_for_destroy_;
};
