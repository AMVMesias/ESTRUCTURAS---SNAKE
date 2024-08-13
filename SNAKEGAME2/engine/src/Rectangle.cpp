#include "Rectangle.h"

Rectangle::Rectangle(float x_, float y_, float w_, float h_) 
        : x(x_), y(y_), w(w_), h(h_) {}

Rectangle::Rectangle() 
    : x(0.f), y(0.f), w(0.f), h(0.f) {}

bool Rectangle::operator==(const Rectangle& other) const {
    return (x == other.x && y == other.y && w == other.w && h == other.h);
}

bool Rectangle::CollidesWith(const Rectangle& other) const {
    return !(x + w <= other.x || // El lado derecho de este rectángulo está a la izquierda del otro
             x >= other.x + other.w || // El lado izquierdo de este rectángulo está a la derecha del otro
             y + h <= other.y || // El lado inferior de este rectángulo está arriba del otro
             y >= other.y + other.h); // El lado superior de este rectángulo está abajo del otro
}
