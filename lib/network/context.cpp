#include <spdlog/spdlog.h>

#include <stdexcept>
#include <swarm/components/position.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/network/context.hpp>

namespace swarm::network {
  // Server connection
  Context::Context() {
    if (enet_initialize() != 0) {
      spdlog::error("An error occurred while trying to initialize ENet.");
      throw std::runtime_error("An error occurred while trying to initialize ENet.");
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = Context::PORT;
    this->host = enet_host_create(&address, 4, 2, 0, 0);

    if (this->host == NULL) {
      spdlog::error("An error occurred while trying to create an ENet server host.");
      throw std::runtime_error("An error occurred while trying to create an ENet server host.");
    }

    spdlog::info("Server started on port {}", Context::PORT);

    this->client_id = -1;
    this->peer = nullptr;
  }

  Context::Context(std::string address) {
    if (enet_initialize() != 0) {
      spdlog::error("An error occurred while trying to initialize ENet.");
      throw std::runtime_error("An error occurred while trying to initialize ENet.");
    }

    this->host = enet_host_create(NULL, 1, 2, 0, 0);
    if (this->host == NULL) {
      spdlog::error("An error occurred while trying to create an ENet client host.");
      throw std::runtime_error("An error occurred while trying to create an ENet client host.");
    }

    ENetAddress addr;
    enet_address_set_host(&addr, address.c_str());
    addr.port = Context::PORT;

    this->peer = enet_host_connect(this->host, &addr, 2, 0);
    if (this->peer == NULL) {
      spdlog::error("No available peers for initiating an ENet connection.");
      throw std::runtime_error("No available peers for initiating an ENet connection.");
    }

    spdlog::info("Connected to server at {}:{}", addr.host, addr.port);

    this->client_id = 0;
  }

  Context *Context::update() {
    this->handle_events();
    return this;
  }

  void Context::handle_events() {
    ENetEvent event;
    while (enet_host_service(this->host, &event, 0) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          spdlog::info("Connected to server");
          break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
          PacketType *type = (PacketType *)event.packet->data;
          auto game = managers::Game::get();

          switch (*type) {
            case PACKET_PLAYER_JOIN: {
              int *client_id = (int *)(event.packet->data + sizeof(PacketType));

              if (this->client_id == 0) {
                this->client_id = *client_id;
              } else {
                spdlog::info("Player {} joined", *client_id);
                game->add_player({*client_id});
              }

              break;
            }
            case PACKET_PLAYER_POSITION: {
              // auto position
              //     = components::Position::deserialize(event.packet->data + sizeof(PacketType));
              // uint16_t client_id = *(uint16_t *)(event.packet->data + sizeof(PacketType)
              //                                    + sizeof(components::Position));
              //
              // spdlog::info("Received player position update: [{}, {}] from player {}",
              //              position.translation.x, position.translation.y, client_id);
              break;
            }
            case PACKET_PLAYER_INPUT: {
              break;
            }
            case PACKET_PLAYER_LEAVE: {
              break;
            }
          }

          break;
        }
        case ENET_EVENT_TYPE_DISCONNECT: {
          spdlog::info("Disconnected from server");
          break;
        }
        case ENET_EVENT_TYPE_NONE: {
          spdlog::info("No event");
          break;
        }
      }
    }
  }

  void Context::send(const void *data, size_t data_size, PacketType type, bool reliable) {
    if (!peer) return;

    ENetPacket *packet = enet_packet_create(NULL, data_size + sizeof(PacketType),
                                            reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

    *(PacketType *)packet->data = type;
    memcpy(packet->data + sizeof(PacketType), data, data_size);

    enet_peer_send(peer, 0, packet);
  }

  void Context::send(NetworkPayload &payload, bool reliable) {
    if (!peer) return;
    spdlog::info("Sending bytes: {}", (char *)payload.serialize().data());
    enet_peer_send(peer, 0, payload.packet(reliable));
  }

  void Context::broadcast(const void *data, size_t data_size, PacketType type, bool reliable) {
    ENetPacket *packet = enet_packet_create(NULL, data_size + sizeof(PacketType),
                                            reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

    *(PacketType *)packet->data = type;
    memcpy(packet->data + sizeof(PacketType), data, data_size);

    enet_host_broadcast(host, 0, packet);
  }

  Context::~Context() {
    if (this->peer != nullptr) enet_peer_disconnect_now(peer, 0);
    enet_host_destroy(host);
    enet_deinitialize();
  }
}  // namespace swarm::network
