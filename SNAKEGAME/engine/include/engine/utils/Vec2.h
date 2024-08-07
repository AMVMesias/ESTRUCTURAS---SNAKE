#pragma once

class Vec2 {
public:
    float x, y;

    Vec2() : x(0.f), y(0.f) {}
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator*=(float scalar);
    Vec2& operator/=(float scalar);

    float Length() const;
    float LengthSquared() const;
    Vec2 Normalized() const;
    float Dot(const Vec2& other) const;
};
