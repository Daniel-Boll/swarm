#pragma once

#include <raylib-cpp/raylib-cpp.hpp>

namespace swarm::components {
  struct Controller {
    KeyboardKey up = KEY_W;
    KeyboardKey left = KEY_A;
    KeyboardKey right = KEY_D;
    KeyboardKey down = KEY_S;
  };
}  // namespace swarm::components
