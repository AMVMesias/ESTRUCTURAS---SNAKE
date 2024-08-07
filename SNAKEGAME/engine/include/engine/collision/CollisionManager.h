#pragma once

#include "ICollidable.h"

#include <vector>

class CollisionManager {
public:
    void AddCollidable(ICollidable& collidable);
    void RemoveCollidable(ICollidable& collidable);
    void CheckCollisions();

private:
    std::vector<ICollidable*> collidables_;
};
