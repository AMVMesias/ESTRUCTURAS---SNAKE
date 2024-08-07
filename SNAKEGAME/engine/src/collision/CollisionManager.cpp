#include "engine/collision/CollisionManager.h"

void CollisionManager::AddCollidable(ICollidable& collidable) {
    collidables_.emplace_back(&collidable);
}

void CollisionManager::RemoveCollidable(ICollidable& collidable) {
    collidables_.erase(
        std::remove(collidables_.begin(), collidables_.end(), &collidable)
    );
}

void CollisionManager::CheckCollisions() {
    auto are_able_to_collide = [](const ICollidable& a, const ICollidable& b) {
        return (a.GetLayer() & b.GetMask() && b.GetLayer() & a.GetMask());
    };

    for (std::size_t i = 0; i < collidables_.size(); ++i) {
        for (std::size_t j = i + 1; j < collidables_.size(); ++j) {
            if (!are_able_to_collide(*collidables_[i], *collidables_[j])) {
                continue;
            }
            
            auto are_colliding = collidables_[i]->GetBoundingBox().CollidesWith(
                collidables_[j]->GetBoundingBox());
            if (are_colliding) {
                collidables_[i]->OnCollision(*collidables_[j]);
                collidables_[j]->OnCollision(*collidables_[i]);
            }
        }
    }
}
