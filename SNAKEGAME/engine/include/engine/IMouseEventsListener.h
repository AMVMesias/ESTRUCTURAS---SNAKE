#pragma once

enum class EMouseEventType {
    RIGHT_DOWN,
    RIGHT_UP,
    LEFT_DOWN,
    LEFT_UP
};

class IMouseEventsListener {
public:
    virtual ~IMouseEventsListener() = default;
    virtual void OnMouseEvent(EMouseEventType event_type, int x, int y) = 0;
};
