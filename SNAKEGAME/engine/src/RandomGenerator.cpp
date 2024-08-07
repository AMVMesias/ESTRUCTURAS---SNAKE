#include "RandomGenerator.h"

RandomGenerator::RandomGenerator()
    : random_generator_(std::random_device()()) {}

int RandomGenerator::Generate(int min_included, int max_included) {
    auto distribution = std::uniform_int_distribution(min_included, max_included);
    return distribution(random_generator_);
}

float RandomGenerator::Generate(float min_included, float max_included) {
    auto distribution = std::uniform_real_distribution<float>(min_included, max_included);
    return distribution(random_generator_);
}

void RandomGenerator::ShuffleNumbers(std::vector<int>& numbers) {
    std::shuffle(numbers.begin(), numbers.end(), random_generator_);
}
