#pragma once

#include "engine/Rectangle.h"

class ICollidable {
public:
    virtual ~ICollidable() = default;
    virtual const Rectangle& GetBoundingBox() const = 0;
    virtual void OnCollision(ICollidable& other) = 0;

    virtual unsigned int GetLayer() const = 0;
    virtual unsigned int GetMask() const = 0;
};
