#include <enet/enet.h>
#include <spdlog/spdlog.h>

#include <raylib-cpp/Vector2.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/systems/movement.hpp>

namespace swarm::systems {
  void movement(flecs::entity it, swarm::components::Position &position,
                const swarm::components::Velocity &velocity) {
    components::Position original = position;
    position.translation = position.translation.Add(velocity.velocity.Scale(GetFrameTime()));

    if (original.translation.x != position.translation.x
        || original.translation.y != position.translation.y) {
      it.set<components::Position>(position);
    }
  }
}  // namespace swarm::systems
