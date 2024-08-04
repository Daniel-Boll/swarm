#pragma once

#include <enet/enet.h>
#include <flecs.h>

#include <optional>
#include <raylib-cpp/raylib-cpp.hpp>
#include <swarm/network/manager.hpp>

namespace swarm::managers {
  class Game {
  protected:
    static Game* instance_;

  public:
    network::Manager network;
    flecs::world ecs;

  public:
    Game();
    static Game* get();

    Game* init_systems();
    Game* process();

    flecs::entity get_me();
    flecs::entity get_player_by_network_id(uint32_t id);
    flecs::entity add_player(std::optional<uint32_t> id = std::nullopt,
                             std::optional<raylib::Vector2> position = std::nullopt,
                             std::optional<raylib::Color> color = std::nullopt);
  };
}  // namespace swarm::managers
