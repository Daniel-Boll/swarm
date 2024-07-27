#pragma once

#include <enet/enet.h>
#include <enet/types.h>

#include <string>

namespace swarm::network {
  class Context {
  private:
  public:
    ENetHost *host;
    ENetPeer *peer;
    int client_id;
    // bool is_server;

    enum PacketType {
      PACKET_PLAYER_POSITION,
      PACKET_PLAYER_INPUT,
      PACKET_PLAYER_JOIN,
      PACKET_PLAYER_LEAVE
    };

    static constexpr enet_uint16 PORT = 7777;

  public:
    Context(enet_uint32 host_type);  // Server
    Context(std::string address);    // Client

    Context *update();

    ~Context();

  private:
    void broadcast(const void *data, size_t data_size, PacketType type, bool reliable);
    void handle_events();
  };

}  // namespace swarm::network
