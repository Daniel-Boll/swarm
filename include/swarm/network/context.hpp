#pragma once

#include <enet/enet.h>
#include <enet/types.h>

#include <string>

#include "swarm/network/payload.hpp"

namespace swarm::network {
  class Context {
  private:
  public:
    ENetHost *host;
    ENetPeer *peer;
    int client_id;

    enum PacketType : uint8_t {
      PACKET_PLAYER_POSITION,
      PACKET_PLAYER_INPUT,
      PACKET_PLAYER_JOIN,
      PACKET_PLAYER_LEAVE
    };

    static constexpr enet_uint16 PORT = 7777;

  public:
    Context();                     // Server
    Context(std::string address);  // Client

    Context *update();
    void broadcast(const void *data, size_t data_size, PacketType type, bool reliable);
    void send(const void *data, size_t data_size, PacketType type, bool reliable);
    void send(NetworkPayload &payload, bool reliable);

    ~Context();

  private:
    void handle_events();
  };

}  // namespace swarm::network
