// Source code from https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/Random.h

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
        // Returns a value between 0 and 1:
        //  - sample distribution function 
        //  - divide the number for the maximum number the distribution can have
        return (float) distribution(randomEngine) / (float) std::numeric_limits<uint32_t>::max();
    }

};

std::mt19937 Random::randomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::distribution;

#endif
