#pragma once

#include <algorithm>
#include <random>
#include <vector>

class RandomGenerator {
public:
    RandomGenerator();
    int Generate(int min_included, int max_included);
    float Generate(float min_included, float max_included);

    void ShuffleNumbers(std::vector<int>& numbers);

private:
    std::mt19937 random_generator_;
};
