#pragma once

#include <raylib-cpp/Color.hpp>

namespace swarm::components {
  struct Renderable {
    raylib::Color color;
    float radius;
  };
}  // namespace swarm::components
