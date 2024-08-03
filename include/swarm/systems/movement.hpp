#pragma once

#include <flecs.h>

#include <swarm/components/position.hpp>
#include <swarm/components/stats.hpp>
#include <swarm/components/velocity.hpp>

namespace swarm::systems {
  void movement(flecs::entity it, swarm::components::Position &position,
                const swarm::components::Velocity &velocity);
}
