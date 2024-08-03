#pragma once

#include <cstring>
#include <raylib-cpp/Vector2.hpp>

namespace swarm::components {
  struct Position {
    raylib::Vector2 translation;

    Position operator=(const Position& other) {
      translation = other.translation;
      return *this;
    }

    ~Position() {}
  };
}  // namespace swarm::components
