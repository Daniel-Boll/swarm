#pragma once

#include <raylib-cpp/raylib-cpp.hpp>
#include <swarm/network/packet.hpp>

namespace swarm::network {

  struct PlayerJoinedPacket : public Packet<PacketType::PLAYER_JOINED> {
  public:
    raylib::Vector2 position;
    raylib::Color color = raylib::Color::Blue();
    uint32_t id;

    PlayerJoinedPacket() = default;
    PlayerJoinedPacket(uint32_t id) : id(id) {
      // TODO: Actually I will have to create another packet so the user tells the server after the
      // connection it's position so I can then position it here
      position.x = 400;
      position.y = 300;

      color = raylib::Color::FromHSV(GetRandomValue(100, 360), 0.8f, 0.5f);
    }

    template <class Archive> void serialize(Archive& ar) {
      ar(position.x, position.y, color.r, color.g, color.b, color.a, id);
    }
  };

}  // namespace swarm::network
