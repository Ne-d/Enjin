#ifndef RENDER_H
#define RENDER_H

#include <SDL_render.h>
#include <SDL_video.h>

// A few global values that need to be accessed in many places.
// Not the file I'm most proud of.

namespace Naito {

inline SDL_Window* globalWindow;
inline SDL_Renderer* globalRenderer;

}

#endif //RENDER_H
