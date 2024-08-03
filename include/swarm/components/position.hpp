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

    template <class Archive> void serialize(Archive& ar) { ar(translation.x, translation.y); }

    ~Position() {}
  };
}  // namespace swarm::components
