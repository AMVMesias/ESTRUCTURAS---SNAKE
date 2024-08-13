#include "engine/utils/Vec2.h"

#include <cmath>

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar) const {
    return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(const Vec2& other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

Vec2& Vec2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2& Vec2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

float Vec2::Length() const {
    return std::sqrt(x * x + y * y);
}

float Vec2::LengthSquared() const {
    return x * x + y * y;
}

Vec2 Vec2::Normalized() const {
    float length = Length();
    if (length != 0) {
        return Vec2(x / length, y / length);
    }
    return Vec2(0, 0);
}

float Vec2::Dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}
