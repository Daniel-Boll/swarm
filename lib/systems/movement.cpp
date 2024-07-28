#include <enet/enet.h>
#include <spdlog/spdlog.h>

#include <raylib-cpp/Vector2.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/network/context.hpp>
#include <swarm/systems/movement.hpp>
#include <vector>

namespace swarm::systems {
  void movement(swarm::components::Movement &movement, const swarm::components::Stats &stats) {
    raylib::Vector2 input(0);
    managers::Game *game = managers::Game::get();

    if (IsKeyDown(KEY_W)) input.y -= 1;
    if (IsKeyDown(KEY_S)) input.y += 1;
    if (IsKeyDown(KEY_A)) input.x -= 1;
    if (IsKeyDown(KEY_D)) input.x += 1;
    if (IsKeyDown(KEY_R)) {
      spdlog::info("Reloading");

      std::vector<std::byte> buffer = {
          static_cast<std::byte>(movement.position.x),
          static_cast<std::byte>(movement.position.y),
      };
      game->context->send(buffer.data(), buffer.size(),
                          network::Context::PacketType::PACKET_PLAYER_POSITION, false);
    }

    movement.velocity = (input.x != 0 || input.y != 0) ? input.Normalize().Scale(stats.move_speed)
                                                       : raylib::Vector2::Zero();
    movement.position = movement.position.Add(movement.velocity.Scale(GetFrameTime()));
  }
}  // namespace swarm::systems
