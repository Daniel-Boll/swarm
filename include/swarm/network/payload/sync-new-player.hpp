#pragma once

#include <raylib-cpp/Vector2.hpp>
#include <swarm/components/position.hpp>
#include <swarm/network/packet.hpp>

namespace swarm::network {

  struct SyncNewPlayerPacket : public Packet<PacketType::SYNC_NEW_PLAYER> {
  public:
    raylib::Vector2 position;
    uint32_t to;
    uint32_t id;

    SyncNewPlayerPacket() = default;
    // NOTE: This could eventually expect the whole flecs::entity and pick the components it needs
    SyncNewPlayerPacket(components::Position position, uint32_t to)
        : position(position.translation), to(to) {}
    SyncNewPlayerPacket(const SyncNewPlayerPacket& other, uint32_t id)
        : position(other.position), id(id) {}

    template <class Archive> void serialize(Archive& ar) { ar(position.x, position.y, to, id); }
  };

}  // namespace swarm::network
