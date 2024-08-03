#pragma once

#include <flecs.h>

#include <swarm/components/network.hpp>
#include <swarm/components/position.hpp>

namespace swarm::systems {
  void sync(components::Position& position);
}  // namespace swarm::systems
