#include <swarm/systems/input.hpp>

namespace swarm::systems {
  void input(const swarm::components::Controller &controller, swarm::components::Velocity &velocity,
             const swarm::components::Stats &stats, const swarm::components::Network &network) {
    if (!network.is_local) return;

    auto input = raylib::Vector2{0, 0};

    if (IsKeyDown(controller.up)) input.y -= 1;
    if (IsKeyDown(controller.down)) input.y += 1;
    if (IsKeyDown(controller.left)) input.x -= 1;
    if (IsKeyDown(controller.right)) input.x += 1;

    velocity.velocity = (input.x != 0 || input.y != 0) ? input.Normalize().Scale(stats.move_speed)
                                                       : raylib::Vector2::Zero();
  }
}  // namespace swarm::systems
