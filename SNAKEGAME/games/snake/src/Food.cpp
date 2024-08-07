#include "snake/include/Food.h"

namespace {
    static const unsigned int kLayer = 4;
    static const unsigned int kMask = 1; // Snake's head Layer.
    static const Color kFoodColor{ 255, 153, 153, 255 }; // Smooth red
}

Food::Food(Engine& engine, Rectangle rect)
    : engine_(engine)
    , rect_(rect)
    , is_marked_for_destroy_(false) {
    engine_.GetCollisionManager().AddCollidable(*this);
}

Food::~Food() {
    engine_.GetCollisionManager().RemoveCollidable(*this);
}

bool Food::IsMarkedForDestroy() const {
    return is_marked_for_destroy_;
}

void Food::Render() {
    engine_.DrawRectangle(rect_, kFoodColor, true);
}

const Rectangle& Food::GetBoundingBox() const {
    return rect_;
}

void Food::OnCollision(ICollidable& other) {
    is_marked_for_destroy_ = true;
}

unsigned int Food::GetLayer() const {
    return kLayer;
}

unsigned int Food::GetMask() const {
    return kMask;
}
