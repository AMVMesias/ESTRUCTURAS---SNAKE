#include "snake/include/Snake.h"

#include "snake/include/Food.h"

#include <array>
#include <algorithm>
#include <functional>
#include <ranges>

namespace {
    static const unsigned int kHeadLayer = 1;
    static const unsigned int kHeadMask = 14; // 4 FoodLayer + 2 NodeLayer + 8 BoundsLayer

    static const unsigned int kNodeLayer = 2;
    static const unsigned int kNodeMask = 1; // 1 HeadLayer

    static const float kNodeWidth = 20.f;
    static const float kNodeHeight = 20.f;

    static const float kSnakeStepSeconds = 0.05f;

    static const Color kHeadColor {102, 204, 102, 255}; // Dark Green
    static const Color kBodyColor {153, 255, 153, 255}; // Green
}

Snake::Snake(Engine& engine) 
    : engine_(engine)
    , is_alive_(true)
    , direction_(Snake::EDirection::UP)
    , next_direction_(Snake::EDirection::UP) // If we don't have this, we can change to an unallowed direction 
    , step_timer_(kSnakeStepSeconds) {
    InitSnake();
}

void Snake::InitSnake() {
    Vec2 position {engine_.GetFWindowWidth() * 0.5f, engine_.GetFWindowHeight() * 0.5f};
    Rectangle starting_rect {position.x, position.y, kNodeWidth, kNodeHeight};
    AddNode(starting_rect, kHeadLayer, kHeadMask);

    for (std::size_t i = 1; i <= 2; ++i) {
        starting_rect.y += kNodeHeight;
        AddNode(starting_rect, 0, 0);
    }
}

void Snake::AddNode() {
    AddNode(nodes_.back()->GetBoundingBox(), kNodeLayer, kNodeMask);
}

void Snake::AddNode(Rectangle rect, unsigned int node_layer, unsigned int node_mask) {
    nodes_.emplace_back(std::make_unique<Snake::Node>(engine_, *this, rect, node_layer, node_mask));
}

bool Snake::IsRectangleCollidingWithNode(Rectangle& rect) const {
    return std::any_of(nodes_.begin(), nodes_.end(), [rect](const auto& node) {
        return node->GetBoundingBox().CollidesWith(rect);
    });
}

void Snake::OnKeyboardEvent(EKeyEventType event_type, SDL_Scancode scancode) {
    const bool is_key_down = (event_type == EKeyEventType::KEY_DOWN);
    if (!is_key_down) return;

    switch(scancode) {
        case SDL_SCANCODE_W:
            next_direction_ = EDirection::UP;
        break;
        case SDL_SCANCODE_S:
            next_direction_ = EDirection::DOWN;
        break;
        case SDL_SCANCODE_D:
            next_direction_ = EDirection::RIGHT;
        break;
        case SDL_SCANCODE_A:
            next_direction_ = EDirection::LEFT;
        break;
    }
}

void Snake::SetDirection(EDirection direction) {
    direction_ = direction;
}

void Snake::Update(float dt) {
    step_timer_.Update(dt);
    if (!step_timer_.DidFinish()) return;
    
    if (IsDirectionAllowed(next_direction_)) {
        SetDirection(next_direction_);
    }

    Vec2 delta_movement {kNodeWidth, kNodeHeight};
    switch(direction_) {
        case EDirection::UP:    delta_movement *= Vec2{ 0, -1}; break;
        case EDirection::DOWN:  delta_movement *= Vec2{ 0,  1}; break;
        case EDirection::LEFT:  delta_movement *= Vec2{-1,  0}; break;
        case EDirection::RIGHT: delta_movement *= Vec2{ 1,  0}; break;
        default:                delta_movement *= Vec2{ 0,  0}; break;
    }

    for (std::size_t i = nodes_.size() - 1; i > 0; --i) {
        Vec2 new_position = nodes_[i - 1]->GetPosition();
        nodes_[i]->SetPosition(new_position);
    }
    
    Vec2 new_head_position = nodes_.front()->GetPosition() + delta_movement;
    nodes_[0]->SetPosition(new_head_position);
}

bool Snake::IsDirectionAllowed(EDirection direction) const {
    std::array<bool, 4> allowed_directions {{
        {direction_ == EDirection::UP    && direction != EDirection::DOWN},
        {direction_ == EDirection::DOWN  && direction != EDirection::UP},
        {direction_ == EDirection::LEFT  && direction != EDirection::RIGHT},
        {direction_ == EDirection::RIGHT && direction != EDirection::LEFT}
    }};

    return std::any_of(allowed_directions.begin(), allowed_directions.end(), std::identity{});
}

void Snake::Render() {
    for (std::size_t i = 0; i < nodes_.size(); ++i) {
        const auto& color = (i == 0) ? kHeadColor : kBodyColor;
        engine_.DrawRectangle(nodes_[i]->GetBoundingBox(), color, true);
    }
}

void Snake::Die() {
    is_alive_ = false;
}

bool Snake::IsAlive() const {
    return is_alive_;
}

void Snake::Reset() {
    nodes_.clear();
    step_timer_.Reset();
    is_alive_ = true;
    direction_ = EDirection::UP;
    next_direction_ = EDirection::UP;
    InitSnake();
}

// ########### Snake Node ########### 

Snake::Node::Node(Engine& engine, Snake& snake, Rectangle rect, unsigned int layer, unsigned int mask) 
    : engine_(engine)
    , snake_(snake)
    , rect_(rect)
    , layer_(layer)
    , mask_(mask) {
    engine_.GetCollisionManager().AddCollidable(*this);
}

Snake::Node::~Node() {
    engine_.GetCollisionManager().RemoveCollidable(*this);
}

Vec2 Snake::Node::GetPosition() const {
    return {rect_.x, rect_.y};
}

void Snake::Node::SetPosition(Vec2 position) {
    rect_.x = position.x;
    rect_.y = position.y;
}

const Rectangle& Snake::Node::GetBoundingBox() const {
    return rect_;
}

void Snake::Node::OnCollision(ICollidable& other) {
    if (dynamic_cast<Food*>(&other)) {
        snake_.AddNode();
    } else {
        snake_.Die();
    }
}

unsigned int Snake::Node::GetLayer() const {
    return layer_;
}

unsigned int Snake::Node::GetMask() const {
    return mask_;
}
