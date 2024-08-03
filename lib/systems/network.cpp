#include <spdlog/spdlog.h>

#include <swarm/manager/game.hpp>
#include <swarm/network/context.hpp>
#include <swarm/systems/network.hpp>

#include "swarm/network/position.hpp"

namespace swarm::systems {
  void sync(components::Position& position) {
    auto game = managers::Game::get();
    network::PositionIn packet{network::Context::PACKET_PLAYER_POSITION, position};

    game->context.send(packet, false);
  }
}  // namespace swarm::systems
