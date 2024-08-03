#include <cstdlib>
#include <swarm/components/network.hpp>
#include <swarm/systems/render.hpp>

namespace swarm::systems {
  void render(const swarm::components::Renderable& renderable,
              const swarm::components::Position& position,
              const swarm::components::Network& network) {
    if (network.is_local) {
      position.translation.DrawCircle(renderable.radius + 1, raylib::Color::RayWhite());
    }
    position.translation.DrawCircle(renderable.radius, renderable.color);
  }
}  // namespace swarm::systems
