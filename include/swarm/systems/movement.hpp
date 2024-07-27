#pragma once

#include <swarm/components/movement.hpp>
#include <swarm/components/stats.hpp>

namespace swarm::systems {
  void movement(swarm::components::Movement& movement, const swarm::components::Stats& stats);
}
