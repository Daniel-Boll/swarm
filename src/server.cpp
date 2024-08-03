// ————— No reconciliation? —————
// ⠀⣞⢽⢪⢣⢣⢣⢫⡺⡵⣝⡮⣗⢷⢽⢽⢽⣮⡷⡽⣜⣜⢮⢺⣜⢷⢽⢝⡽⣝
// ⠸⡸⠜⠕⠕⠁⢁⢇⢏⢽⢺⣪⡳⡝⣎⣏⢯⢞⡿⣟⣷⣳⢯⡷⣽⢽⢯⣳⣫⠇
// ⠀⠀⢀⢀⢄⢬⢪⡪⡎⣆⡈⠚⠜⠕⠇⠗⠝⢕⢯⢫⣞⣯⣿⣻⡽⣏⢗⣗⠏⠀
// ⠀⠪⡪⡪⣪⢪⢺⢸⢢⢓⢆⢤⢀⠀⠀⠀⠀⠈⢊⢞⡾⣿⡯⣏⢮⠷⠁⠀⠀
// ⠀⠀⠀⠈⠊⠆⡃⠕⢕⢇⢇⢇⢇⢇⢏⢎⢎⢆⢄⠀⢑⣽⣿⢝⠲⠉⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⡿⠂⠠⠀⡇⢇⠕⢈⣀⠀⠁⠡⠣⡣⡫⣂⣿⠯⢪⠰⠂⠀⠀⠀⠀
// ⠀⠀⠀⠀⡦⡙⡂⢀⢤⢣⠣⡈⣾⡃⠠⠄⠀⡄⢱⣌⣶⢏⢊⠂⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⢝⡲⣜⡮⡏⢎⢌⢂⠙⠢⠐⢀⢘⢵⣽⣿⡿⠁⠁⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠨⣺⡺⡕⡕⡱⡑⡆⡕⡅⡕⡜⡼⢽⡻⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⣼⣳⣫⣾⣵⣗⡵⡱⡡⢣⢑⢕⢜⢕⡝⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⣴⣿⣾⣿⣿⣿⡿⡽⡑⢌⠪⡢⡣⣣⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⡟⡾⣿⢿⢿⢵⣽⣾⣼⣘⢸⢸⣞⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠁⠇⠡⠩⡫⢿⣝⡻⡮⣒⢽⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// —————————————————————————————

#include <enet/enet.h>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <cstring>
#include <swarm/components/position.hpp>
#include <swarm/network/context.hpp>
#include <swarm/network/position.hpp>
#include <vector>

using namespace swarm;

void network_broadcast(network::Context* context, const void* data, size_t data_size,
                       network::Context::PacketType type, bool reliable);

int main() {
  network::Context context;
  std::vector<ENetPeer*> peers;

  while (true) {
    ENetEvent event;
    while (enet_host_service(context.host, &event, 0) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          peers.push_back(event.peer);

          spdlog::info("A new client connected from {}:{}.", event.peer->address.host,
                       event.peer->address.port);

          size_t client_id = context.host->connectedPeers;

          std::vector<std::byte> buffer{
              static_cast<std::byte>(network::Context::PacketType::PACKET_PLAYER_JOIN),
              static_cast<std::byte>(client_id)};

          network_broadcast(&context, buffer.data(), buffer.size(),
                            network::Context::PACKET_PLAYER_JOIN, true);
          break;
        }
        case ENET_EVENT_TYPE_DISCONNECT: {
          spdlog::info("Client {} disconnected", event.peer->incomingPeerID);
          break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
          network::Context::PacketType* type = (network::Context::PacketType*)event.packet->data;

          switch (*type) {
            case network::Context::PACKET_PLAYER_POSITION: {
              std::vector<std::byte> buffer;
              buffer.resize(event.packet->dataLength);
              std::memcpy(buffer.data(), event.packet->data, event.packet->dataLength);

              spdlog::info("Got bytes: {}", (char*)event.packet->data);

              network::PositionIn data;
              network::PositionIn::deserialize(buffer, data);

              spdlog::info("Received position: ({}, {}) from [{}]", data.position.translation.x,
                           data.position.translation.y, event.peer->outgoingPeerID);

              // add the client id to the data and broadcast it
              // uint16_t client_id = event.peer->incomingPeerID;
              // std::vector<std::byte> buffer(event.packet->dataLength + sizeof(uint16_t));
              // std::memcpy(buffer.data(), event.packet->data, event.packet->dataLength);
              // std::memcpy(buffer.data() + event.packet->dataLength, &client_id,
              // sizeof(uint16_t));
              //
              // context.broadcast(buffer.data(), buffer.size(),
              //                   network::Context::PACKET_PLAYER_POSITION, false);

              break;
            }
            case network::Context::PACKET_PLAYER_INPUT: {
              break;
            }
            case network::Context::PACKET_PLAYER_LEAVE: {
              break;
            }
            case network::Context::PACKET_PLAYER_JOIN: {
              break;
            }
          }

          break;
        }
        case ENET_EVENT_TYPE_NONE: {
          spdlog::info("No event");
          break;
        }
      }
    }
  }

  return 0;
}

void network_broadcast(network::Context* context, const void* data, size_t data_size,
                       network::Context::PacketType type, bool reliable) {
  ENetPacket* packet = enet_packet_create(NULL, data_size + sizeof(network::Context::PacketType),
                                          reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

  *(network::Context::PacketType*)packet->data = type;
  memcpy(packet->data + sizeof(network::Context::PacketType), data, data_size);

  enet_host_broadcast(context->host, 0, packet);
}
