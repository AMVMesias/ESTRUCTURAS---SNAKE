#pragma once

#include "engine/Engine.h"
#include "engine/Rectangle.h"
#include "engine/collision/ICollidable.h"
#include "engine/timer/CountdownTimer.h"
#include "engine/utils/Vec2.h"

#include <vector>
#include <memory>

class Snake : public IKeyboardEventsListener {
public:
    Snake(Engine& engine);
    void Reset();

    void Update(float dt);
    void Render();

    bool IsAlive() const;
    void Die();
    
    void AddNode();
    void AddNode(Rectangle rect, unsigned int node_layer, unsigned int node_mask);
    bool IsRectangleCollidingWithNode(Rectangle& rect) const;

    // IKeyboardEventsListener
    void OnKeyboardEvent(EKeyEventType event_type, SDL_Scancode scancode) override;

private:
    enum class EDirection {
        UP, DOWN, LEFT, RIGHT
    };

    class Node : ICollidable {
    public:
        Node(Engine& engine, Snake& snake, Rectangle rect, unsigned int layer, unsigned int mask);
        ~Node();

        Vec2 GetPosition() const;
        void SetPosition(Vec2 position);

        // ICollidable
        const Rectangle& GetBoundingBox() const override; 
        void OnCollision(ICollidable& other) override; 
        unsigned int GetLayer() const override; 
        unsigned int GetMask() const override; 
    
    private:
        Engine& engine_;
        Snake& snake_;
        Rectangle rect_;
        unsigned int layer_;
        unsigned int mask_;
    };

    Engine& engine_;
    bool is_alive_;
    EDirection direction_;
    EDirection next_direction_;
    CountdownTimer step_timer_;
    std::vector<std::unique_ptr<Node>> nodes_;

    void InitSnake();
    bool IsDirectionAllowed(EDirection direction) const;
    void SetDirection(EDirection direction);
};
