#include <cstdlib>
#include <swarm/systems/render.hpp>

namespace swarm::systems {
  void render(const swarm::components::Renderable& renderable,
              const swarm::components::Position& position) {
    position.translation.DrawCircle(renderable.radius, renderable.color);
  }
}  // namespace swarm::systems
