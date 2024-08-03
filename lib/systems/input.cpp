#include <swarm/systems/input.hpp>

namespace swarm::systems {
  void input(const swarm::components::Controller &movement, swarm::components::Velocity &velocity,
             const swarm::components::Stats &stats) {
    auto input = raylib::Vector2{0, 0};

    if (IsKeyDown(movement.up)) input.y -= 1;
    if (IsKeyDown(movement.down)) input.y += 1;
    if (IsKeyDown(movement.left)) input.x -= 1;
    if (IsKeyDown(movement.right)) input.x += 1;

    velocity.velocity = (input.x != 0 || input.y != 0) ? input.Normalize().Scale(stats.move_speed)
                                                       : raylib::Vector2::Zero();
  }
}  // namespace swarm::systems
