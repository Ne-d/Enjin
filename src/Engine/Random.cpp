#include "Random.h"

#include <random>


namespace Naito {

std::random_device randomDevice;
std::mt19937 gen{randomDevice()};

int randomInt(const int min, const int max) {
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

float randomFloat(const float min, const float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

short randomDirection() {
    std::uniform_int_distribution<> dist(0, 1);

    if (dist(gen) == 0)
        return -1;

    return 1;
}


} // Naito
