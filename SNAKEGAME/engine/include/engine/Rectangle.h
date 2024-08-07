#pragma once

class Rectangle {
public:
    Rectangle(float x_, float y_, float w_, float h_);
    Rectangle();
    
    bool CollidesWith(const Rectangle& other) const;
    bool operator==(const Rectangle& other) const;

    float x, y, w, h;
};
