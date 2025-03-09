#include "Cell.h"

#include "Random.h"
#include "World.h"


namespace Naito {

SDL_Color cellColor(const Cell cell) {
    switch (cell.element) {
    case Element::Sand:
        return SDL_Color{255, 255, 255};

    default:
        return SDL_Color{0, 0, 0};
    }
}
} // Naito
