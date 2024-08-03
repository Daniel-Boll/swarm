#pragma once

#include <cstdint>

namespace swarm::components {
  struct Network {
    uint32_t id;
    bool is_local;
  };
}  // namespace swarm::components
