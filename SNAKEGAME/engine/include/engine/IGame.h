#pragma once

#include "IMouseEventsListener.h"
#include "IKeyboardEventsListener.h"

class IGame : public IMouseEventsListener, public IKeyboardEventsListener {
public:
    virtual ~IGame() = default;
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
};
