#pragma once

#include <swarm/components/movement.hpp>
#include <swarm/components/renderable.hpp>

namespace swarm::systems {
  void render(const swarm::components::Renderable& renderable,
              const swarm::components::Movement& movement);
}
