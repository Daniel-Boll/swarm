#pragma once

#include <enet/enet.h>

#include <cereal/types/polymorphic.hpp>
#include <swarm/components/position.hpp>
#include <swarm/network/context.hpp>
#include <swarm/network/payload.hpp>

namespace swarm::network {
  struct PositionIn : NetworkPayload {
    Context::PacketType packet;
    components::Position position;

    PositionIn() = default;

    PositionIn(Context::PacketType packet, components::Position position)
        : packet(packet), position(position) {}

    template <class Archive> void serialize(Archive& ar) { ar(packet, position); }
  };

  struct PositionOut : NetworkPayload {
    Context::PacketType packet;
    components::Position position;
    int client_id;

    PositionOut(components::Position position) : position(position) {}

    template <class Archive> void serialize(Archive& ar) { ar(packet, position); }
  };
}  // namespace swarm::network

CEREAL_REGISTER_TYPE(swarm::network::PositionIn)
CEREAL_REGISTER_POLYMORPHIC_RELATION(swarm::network::NetworkPayload, swarm::network::PositionIn)

CEREAL_REGISTER_TYPE(swarm::network::PositionOut)
CEREAL_REGISTER_POLYMORPHIC_RELATION(swarm::network::NetworkPayload, swarm::network::PositionOut)
