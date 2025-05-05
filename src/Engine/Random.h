#ifndef RANDOM_H
#define RANDOM_H


namespace Naito {

/// Generates a random integer with a uniform distribution between min and max, both included.
int randomInt(int min, int max);

/// Generates a random float with a uniform distribution between min and max, both included.
float randomFloat(float min, float max);

/// Generates a random short integer that can be either -1 or 1.
short randomDirection();

} // Naito

#endif //RANDOM_H
