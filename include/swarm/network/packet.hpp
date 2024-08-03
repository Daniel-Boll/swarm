#pragma once

#include <cereal/archives/binary.hpp>

namespace swarm::network {

  enum class PacketType {
    PLAYER_JOINED,
    PLAYER_POSITION,
  };

  class BasePacket {
  public:
    virtual ~BasePacket() = default;
    virtual PacketType getType() const = 0;

    template <class Archive> void serialize(Archive& ar) {}
  };

  template <PacketType Type> class Packet : public BasePacket {
  public:
    PacketType getType() const override { return Type; }

    std::string toString() const {
      switch (Type) {
        case PacketType::PLAYER_JOINED:
          return "PLAYER_JOINED";
        case PacketType::PLAYER_POSITION:
          return "PLAYER_POSITION";
        default:
          return "UNKNOWN";
      }
    }
  };

}  // namespace swarm::network
