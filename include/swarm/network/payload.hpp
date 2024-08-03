
#pragma once

#include <enet/enet.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <sstream>
#include <vector>

namespace swarm::network {
  class NetworkPayload {
  public:
    virtual std::vector<std::byte> serialize() const {
      std::stringstream ss;
      cereal::BinaryOutputArchive oarchive(ss);
      oarchive(*this);

      auto data = std::vector<std::byte>(ss.str().size());
      std::memcpy(data.data(), ss.str().data(), ss.str().size());
      return data;
    }

    static std::vector<std::byte> serialize(const NetworkPayload& payload) {
      std::stringstream ss;
      cereal::BinaryOutputArchive oarchive(ss);
      oarchive(payload);

      auto data = std::vector<std::byte>(ss.str().size());
      std::memcpy(data.data(), ss.str().data(), ss.str().size());
      return data;
    }

    template <typename T> static T deserialize(std::vector<std::byte>& data) {
      T payload;
      std::string str(data.size(), '\0');
      std::transform(data.begin(), data.end(), str.begin(), [](std::byte b) { return char(b); });
      std::stringstream ss(str);
      cereal::BinaryInputArchive iarchive(ss);
      iarchive(payload);
      return payload;
    }

    static void deserialize(const std::vector<std::byte>& data, NetworkPayload& payload) {
      std::string str(reinterpret_cast<const char*>(data.data()), data.size());
      std::stringstream ss(str);
      cereal::BinaryInputArchive iarchive(ss);
      iarchive(payload);
    }

    ENetPacket* packet(bool reliable) const {
      auto bytes = serialize();
      ENetPacket* packet
          = enet_packet_create(nullptr, bytes.size(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
      std::memcpy(packet->data, bytes.data(), bytes.size());
      return packet;
    }

    virtual ~NetworkPayload() = default;

    // Make NetworkPayload polymorphic with Cereal
    template <class Archive> void serialize(Archive& ar) {
      // No need to serialize base class members, just enable polymorphism
    }
  };
}  // namespace swarm::network
