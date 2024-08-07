#pragma once

#include "engine/Engine.h"
#include "engine/utils/Vec2.h"

#include "Snake.h"
#include "Food.h"

#include <memory>

class FoodFactory {
public:
    FoodFactory(Engine& engine, const Snake& snake);

    std::unique_ptr<Food> CreateFood();

private:
    Engine& engine_;
    const Snake& snake_;

    std::vector<Vec2> spawn_positions_;
    void InitSpawnPositions();
};
