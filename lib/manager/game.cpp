#include <enet/enet.h>
#include <spdlog/spdlog.h>

#include <raylib-cpp/Vector2.hpp>
#include <swarm/components/all.hpp>
#include <swarm/components/renderable.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/network/manager.hpp>
#include <swarm/network/payload/player-joined.hpp>
#include <swarm/network/payload/player-position.hpp>
#include <swarm/network/payload/sync-new-player.hpp>
#include <swarm/systems/input.hpp>
#include <swarm/systems/movement.hpp>
#include <swarm/systems/network.hpp>
#include <swarm/systems/render.hpp>

namespace swarm::managers {
  Game* Game::instance_ = nullptr;

  Game::Game() {
    ecs = flecs::world();
    if (!network.connectToServer("localhost", 12345)) {
      spdlog::error("Failed to connect to server\n");
      throw std::runtime_error("Failed to connect to server");
    }
  }

  Game* Game::get() {
    if (instance_ == nullptr) instance_ = new Game();

    return instance_;
  }

  flecs::entity Game::get_me() {
    return ecs.query<components::Network>().find(
        [](components::Network& network) { return network.is_local; });
  }

  flecs::entity Game::get_player_by_network_id(uint32_t id) {
    return ecs.query<components::Network>().find(
        [&id](components::Network& network) { return network.id == id; });
  }

  Game* Game::init_systems() {
    // clang-format off
    ecs.system<components::Position, const components::Velocity>()
      .each(systems::movement);

    ecs.system<const components::Renderable, const components::Position, const components::Network>()
      .each(systems::render);

    ecs.system<const components::Controller, components::Velocity, const components::Stats, const components::Network>()
      .each(systems::input);

    ecs.observer<const components::Position, const components::Network>()
      .event(flecs::OnSet)
      .each(systems::sync);
    // clang-format on

    network.registerPacketHandler<network::PlayerJoinedPacket>(
        [this](const network::PlayerJoinedPacket& packet, ENetPeer* peer) {
          add_player(packet.id, packet.position, packet.color);

          auto current_player = get_me();

          auto position = current_player.get<components::Position>();
          // auto color = current_player.get<components::Renderable>()->color;

          // Update the user with my info
          this->network.sendToServer<network::SyncNewPlayerPacket>({*position, packet.id});
        });

    network.registerPacketHandler<network::SyncNewPlayerPacket>(
        [this](const network::SyncNewPlayerPacket& packet, ENetPeer* peer) {
          auto player = get_player_by_network_id(packet.id);

          if (!player) {
            spdlog::error("Player with id {} does not exist\n", packet.id);
            return;
          }

          player.set<components::Position>({packet.position});
        });

    network.registerPacketHandler<network::PlayerPositionPacket>(
        [this](const network::PlayerPositionPacket& packet, ENetPeer* peer) {
          auto player = get_player_by_network_id(packet.id);

          if (!player) {
            spdlog::error("Player with id {} not found\n", packet.id);
            return;
          }

          player.set<components::Position>({packet.position});
        });

    return this;
  }

  Game* Game::process() {
    ecs.progress(GetFrameTime());
    network.update();
    return this;
  }

  flecs::entity Game::add_player(std::optional<uint32_t> client_id,
                                 std::optional<raylib::Vector2> position,
                                 std::optional<raylib::Color> color) {
    return ecs.entity()
        .set<components::Renderable>({color.value_or(raylib::Color::Red()), 10})
        .set<components::Position>({position.value_or(raylib::Vector2{400, 300})})
        .set<components::Velocity>({0})
        .set<components::Controller>({})
        .set<components::Network>({client_id.value_or(0), !client_id.has_value()})
        .set<components::Stats>({100, 100, 10, 200, 0, 1, 1, 0.05, 1, 1});
  }
}  // namespace swarm::managers
