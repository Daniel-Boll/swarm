#include <spdlog/spdlog.h>

#include <stdexcept>
#include <swarm/network/context.hpp>

namespace swarm::network {
  // Server connection
  Context::Context(enet_uint32 host_type) {
    if (enet_initialize() != 0) {
      spdlog::error("An error occurred while trying to initialize ENet.");
      throw std::runtime_error("An error occurred while trying to initialize ENet.");
    }

    ENetAddress addr;
    enet_address_set_host(&addr, "localhost");
    addr.port = Context::PORT;
    this->host = enet_host_create(&addr, 4, 2, 0, 0);

    if (this->host == NULL) {
      spdlog::error("An error occurred while trying to create an ENet server host.");
      throw std::runtime_error("An error occurred while trying to create an ENet server host.");
    }

    spdlog::info("Server started on port {}", Context::PORT);

    this->client_id = -1;
    this->peer = nullptr;
    // this->is_server = true;
  }

  Context::Context(std::string address) {
    if (enet_initialize() != 0) {
      spdlog::error("An error occurred while trying to initialize ENet.");
      throw std::runtime_error("An error occurred while trying to initialize ENet.");
    }

    this->host = enet_host_create(NULL, 1, 2, 0, 0);
    if (this->host == NULL) {
      spdlog::error("An error occurred while trying to create an ENet client host.");
      std::runtime_error("An error occurred while trying to create an ENet client host.");
    }

    ENetAddress addr;
    enet_address_set_host(&addr, address.c_str());
    addr.port = Context::PORT;

    this->peer = enet_host_connect(this->host, &addr, 2, 0);
    if (this->peer == NULL) {
      spdlog::error("No available peers for initiating an ENet connection.");
      std::runtime_error("No available peers for initiating an ENet connection.");
    }

    // enet_host_flush(this->host);

    spdlog::info("Connected to server at {}:{}", addr.host, addr.port);

    this->client_id = 0;
    // this->is_server = false;
  }

  Context* Context::update() {
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
          PacketType* type = (PacketType*)event.packet->data;
          switch (*type) {
            case PACKET_PLAYER_JOIN: {
              int* client_id = (int*)(event.packet->data + sizeof(PacketType));
              spdlog::info("Player {} joined", *client_id);

              break;
            }
            case PACKET_PLAYER_POSITION: {
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
          break;
        }
        case ENET_EVENT_TYPE_NONE: {
          spdlog::info("No event");
          break;
        }
      }
    }
  }

  Context::~Context() {
    if (this->peer != nullptr) enet_peer_disconnect_now(peer, 0);
    enet_host_destroy(host);
    enet_deinitialize();
  }
}  // namespace swarm::network
