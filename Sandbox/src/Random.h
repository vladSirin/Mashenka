#pragma once
#include <random>

class Random {
public:
    // Initializes the random engine. Call this at the start of your program.
    static void Init() {
        s_RandomEngine.seed(std::random_device()());
    }

    // Generates a floating-point number between 0.0 and 1.0.
    static float Float() {
        std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        return distribution(s_RandomEngine);
    }

    // Generates an integer number within [min, max].
    static int Int(int min, int max) {
        std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);
        return distribution(s_RandomEngine);
    }

private:
    static std::mt19937 s_RandomEngine;
};
