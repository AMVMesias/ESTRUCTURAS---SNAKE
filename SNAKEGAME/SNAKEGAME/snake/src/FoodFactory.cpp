#include "snake/include/FoodFactory.h"

namespace {
    static const float kFoodWidth = 10.f;
    static const float kFoodHeight = 10.f;
}

FoodFactory::FoodFactory(Engine& engine, const Snake& snake) 
    : engine_(engine)
    , snake_(snake) {
    InitSpawnPositions();
}

void FoodFactory::InitSpawnPositions() {
    const float every_x = 25.f;
    const float every_y = 25.f;
    for (float x = every_x; x < engine_.GetFWindowWidth(); x += every_x) {
        for (float y = every_y; y < engine_.GetFWindowHeight(); y += every_y) {
            spawn_positions_.emplace_back(x, y);
        }
    }
}

std::unique_ptr<Food> FoodFactory::CreateFood() {
    auto& random_generator = engine_.GetRandomGenerator();
    Rectangle random_rect{0.f, 0.f, kFoodWidth, kFoodHeight};
    auto spawn_positions_count = static_cast<int>(spawn_positions_.size());
    do {
        const auto random_index = static_cast<std::size_t>(
            random_generator.Generate(0, spawn_positions_count));
        const auto random_position = spawn_positions_[random_index];            
        random_rect.x = random_position.x;
        random_rect.y = random_position.y;
    } while(snake_.IsRectangleCollidingWithNode(random_rect));

    return std::make_unique<Food>(engine_, random_rect);
}
