#pragma once

#include <enet/enet.h>
#include <flecs.h>

#include <optional>
#include <swarm/network/context.hpp>

namespace swarm::managers {
  class Game {
  protected:
    static Game* instance_;

  public:
    network::Context context{"localhost"};
    flecs::world ecs;

  public:
    Game();
    static Game* get();

    Game* init_systems();
    Game* process();

    flecs::entity add_player(std::optional<uint32_t> id = std::nullopt);
  };
}  // namespace swarm::managers
