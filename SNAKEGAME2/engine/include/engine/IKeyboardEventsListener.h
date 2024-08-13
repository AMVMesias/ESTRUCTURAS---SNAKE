#pragma once

#include <SDL2/SDL.h>

enum class EKeyEventType {
    KEY_UP,
    KEY_DOWN
};

class IKeyboardEventsListener {
public:
    virtual ~IKeyboardEventsListener() = default;
    virtual void OnKeyboardEvent(EKeyEventType event_type, SDL_Scancode scancode) = 0;
};
