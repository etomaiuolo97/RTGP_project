#pragma once

#ifndef RANDOM
#define RANDOM

#include <random>

class Random {
private:
    static std::mt19937 randomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> distribution;

public:
    static void Init () {
        randomEngine.seed(std::random_device()());
    }

    static float Float () {
        return (float) distribution(randomEngine) / (float) std::numeric_limits<uint32_t>::max();
    }

};

std::mt19937 Random::randomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::distribution;

#endif
