#pragma once

#include <enet/enet.h>
#include <flecs.h>

#include <memory>
#include <swarm/network/context.hpp>

namespace swarm::managers {
  class Game {
  protected:
    static Game* instance_;

  public:
    std::unique_ptr<network::Context> context;
    flecs::world ecs;

  public:
    Game();
    static Game* get();

    Game* init_systems();
    Game* process();

    flecs::entity add_player();
  };
}  // namespace swarm::managers
