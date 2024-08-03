#pragma once

#include <raylib-cpp/Vector2.hpp>
#include <swarm/components/position.hpp>
#include <swarm/network/packet.hpp>

namespace swarm::network {

  struct PlayerPositionPacket : public Packet<PacketType::PLAYER_POSITION> {
  public:
    raylib::Vector2 position;
    uint32_t id;

    PlayerPositionPacket() = default;
    PlayerPositionPacket(components::Position position) : position(position.translation) {}
    PlayerPositionPacket(const PlayerPositionPacket& other, uint32_t id)
        : position(other.position), id(id) {}

    template <class Archive> void serialize(Archive& ar) { ar(position.x, position.y, id); }
  };

}  // namespace swarm::network
