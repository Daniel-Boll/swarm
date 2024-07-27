#include <enet/enet.h>

#include <memory>
#include <swarm/components/movement.hpp>
#include <swarm/components/renderable.hpp>
#include <swarm/components/stats.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/network/context.hpp>
#include <swarm/systems/movement.hpp>
#include <swarm/systems/render.hpp>

namespace swarm::managers {
  Game* Game::instance_ = nullptr;

  Game::Game() {
    this->context = std::make_unique<network::Context>(network::Context("localhost"));
    this->ecs = flecs::world();
  }

  Game* Game::get() {
    if (instance_ == nullptr) instance_ = new Game();

    return instance_;
  }

  Game* Game::init_systems() {
    this->ecs.system<swarm::components::Movement, const swarm::components::Stats>().each(
        systems::movement);
    this->ecs.system<const swarm::components::Renderable, const swarm::components::Movement>().each(
        systems::render);

    return this;
  }

  Game* Game::process() {
    this->ecs.progress(GetFrameTime());
    this->context->update();
    return this;
  }

  flecs::entity Game::add_player() {
    return ecs.entity()
        .set<components::Movement>({{400, 300}, {0, 0}})
        .set<components::Renderable>({raylib::Color::Red(), 10})
        .set<components::Stats>({100, 100, 10, 200, 0, 1, 1, 0.05, 1, 1});
  }
}  // namespace swarm::managers
