#pragma once

#include <flecs.h>

#include <swarm/components/network.hpp>
#include <swarm/components/position.hpp>

namespace swarm::systems {
  void sync(const components::Position& position, const components::Network&);
}  // namespace swarm::systems
