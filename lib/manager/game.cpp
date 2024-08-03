#include <enet/enet.h>
#include <spdlog/spdlog.h>

#include <swarm/components/all.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/network/context.hpp>
#include <swarm/systems/input.hpp>
#include <swarm/systems/movement.hpp>
#include <swarm/systems/network.hpp>
#include <swarm/systems/render.hpp>

namespace swarm::managers {
  Game* Game::instance_ = nullptr;

  Game::Game() { ecs = flecs::world(); }

  Game* Game::get() {
    if (instance_ == nullptr) instance_ = new Game();

    return instance_;
  }

  Game* Game::init_systems() {
    // clang-format off
    ecs.system<components::Position, const components::Velocity>()
      .each(systems::movement);

    ecs.system<const components::Renderable, const components::Position>()
      .each(systems::render);

    ecs.system<const components::Controller, components::Velocity, const components::Stats>()
      .each(systems::input);

    ecs.observer<components::Position>()
      .event(flecs::OnSet)
      .each(systems::sync);
    // clang-format on

    return this;
  }

  Game* Game::process() {
    ecs.progress(GetFrameTime());
    context.update();
    return this;
  }

  flecs::entity Game::add_player(std::optional<uint32_t> client_id) {
    return ecs.entity()
        .set<components::Renderable>({raylib::Color::Red(), 10})
        .set<components::Position>({raylib::Vector2{400, 300}})
        .set<components::Velocity>({0})
        .set<components::Controller>({})
        .set<components::Network>({client_id.value_or((uint32_t)this->context.client_id)})
        .set<components::Stats>({100, 100, 10, 200, 0, 1, 1, 0.05, 1, 1});
  }
}  // namespace swarm::managers
