#pragma once

#include <swarm/components/position.hpp>
#include <swarm/components/renderable.hpp>

namespace swarm::systems {
  void render(const swarm::components::Renderable& renderable,
              const swarm::components::Position& movement);
}
