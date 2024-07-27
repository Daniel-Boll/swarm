#pragma once

#include <enet/enet.h>
#include <flecs.h>

#include <memory>
#include <swarm/network/context.hpp>

namespace swarm::managers {
  class Game {
  private:
    std::unique_ptr<network::Context> context;
    flecs::world ecs;

  public:
    Game();

    Game* init_systems();
    Game* process();

    flecs::entity add_player();

    ~Game() = default;
  };
}  // namespace swarm::managers
