#include <raylib-cpp/Vector2.hpp>
#include <swarm/systems/movement.hpp>

namespace swarm::systems {
  void movement(swarm::components::Movement &movement, const swarm::components::Stats &stats) {
    raylib::Vector2 input(0);

    if (IsKeyDown(KEY_W)) input.y -= 1;
    if (IsKeyDown(KEY_S)) input.y += 1;
    if (IsKeyDown(KEY_A)) input.x -= 1;
    if (IsKeyDown(KEY_D)) input.x += 1;

    movement.velocity = (input.x != 0 || input.y != 0) ? input.Normalize().Scale(stats.move_speed)
                                                       : raylib::Vector2::Zero();
    movement.position = movement.position.Add(movement.velocity.Scale(GetFrameTime()));
  }
}  // namespace swarm::systems
