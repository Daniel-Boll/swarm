#pragma once

#include <swarm/components/movement.hpp>
#include <swarm/components/network.hpp>

namespace swarm::systems {
  // Syncs players movement through the network
  void sync(swarm::components::Network& network, const swarm::components::Movement& movement);
}  // namespace swarm::systems
