#pragma once
#include <random>

class UniqueIdGenerator
{
private:
    std::random_device rd;

public:
    UniqueIdGenerator() = default;

    uint64_t CreateNew() {
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(100000, 999999);

        return distribution(gen);
    }
};