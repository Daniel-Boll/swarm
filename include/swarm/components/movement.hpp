#pragma once

#include <raylib-cpp/Vector2.hpp>

namespace swarm::components {
  struct Movement {
    raylib::Vector2 position;
    raylib::Vector2 velocity;
  };
}  // namespace swarm::components
