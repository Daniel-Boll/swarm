#pragma once

#include <swarm/components/controller.hpp>
#include <swarm/components/network.hpp>
#include <swarm/components/stats.hpp>
#include <swarm/components/velocity.hpp>

namespace swarm::systems {
  void input(const swarm::components::Controller& movement, swarm::components::Velocity& velocity,
             const swarm::components::Stats& stats, const swarm::components::Network& network);
}
