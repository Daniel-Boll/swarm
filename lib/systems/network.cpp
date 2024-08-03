#include <spdlog/spdlog.h>

#include <swarm/manager/game.hpp>
#include <swarm/network/payload/player-position.hpp>
#include <swarm/systems/network.hpp>

namespace swarm::systems {
  void sync(const components::Position& position, const components::Network& network) {
    if (!network.is_local) return;

    managers::Game* game = managers::Game::get();

    if (game->network.isConnected()) {
      game->network.sendToServer(network::PlayerPositionPacket{position});
    }
  }
}  // namespace swarm::systems
