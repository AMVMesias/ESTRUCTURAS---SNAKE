#include "snake/include/ScreenBoundary.h"

namespace {
    static const unsigned int kLayer = 8;
    static const unsigned int kMask = 1; // Snake's head Layer.
    static const Color kBoundaryColor {255, 255, 255, 255}; // White
}

ScreenBoundary::ScreenBoundary(Engine& engine, Snake& snake, Rectangle rect) 
    : engine_(engine)
    , snake_(snake)
    , rect_(rect) {
    engine_.GetCollisionManager().AddCollidable(*this);
}

ScreenBoundary::~ScreenBoundary() {
    engine_.GetCollisionManager().RemoveCollidable(*this);
}

void ScreenBoundary::Render() {
    engine_.DrawRectangle(rect_, kBoundaryColor);
}

const Rectangle& ScreenBoundary::GetBoundingBox() const {
    return rect_;
}

void ScreenBoundary::OnCollision(ICollidable& other) {
    snake_.Die();
}

unsigned int ScreenBoundary::GetLayer() const {
    return kLayer;
}

unsigned int ScreenBoundary::GetMask() const {
    return kMask;
}
