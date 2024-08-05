#pragma once

#include <enet/enet.h>
#include <spdlog/spdlog.h>

#include <swarm/network/packet.hpp>

namespace swarm::network {
  class Manager {
  private:
    ENetHost* server = nullptr;
    ENetHost* client = nullptr;
    ENetPeer* serverPeer = nullptr;
    std::unordered_set<ENetPeer*> connectedPeers;
    std::unordered_map<PacketType, std::function<void(const std::vector<uint8_t>&, ENetPeer*)>>
        packetHandlers;

    template <typename T> static std::vector<uint8_t> serializePacket(const T& packet);

    template <typename T> static T deserializePacket(const std::vector<uint8_t>& data);

    void handlePacket(const std::vector<uint8_t>& data, ENetPeer* peer);
    void addPeer(ENetPeer* peer);
    void removePeer(ENetPeer* peer);

  public:
    Manager();
    ~Manager();

    bool createServer(uint16_t port, size_t maxClients);
    bool connectToServer(const char* hostName, uint16_t port);
    bool isConnected() const;
    void disconnect();

    ENetPeer* getPeer(uint32_t id) const;
    template <typename T> void sendPacket(const T& packet, ENetPeer* peer);
    template <typename T> void sendToServer(const T& packet);
    template <typename T> void broadcastPacket(const T& packet);
    template <typename T> void broadcastToOthers(const T& packet, ENetPeer* exclude);

    void update();

    template <typename T>
    void registerPacketHandler(std::function<void(const T&, ENetPeer*)> handler);

    inline bool isServer() const { return server != nullptr; }
    inline bool isClient() const { return client != nullptr; }

    inline ENetPeer* getServerPeer() const { return serverPeer; }
    inline ENetHost* getClient() const { return client; }
  };

  template <typename T> void Manager::sendPacket(const T& packet, ENetPeer* peer) {
    static_assert(std::is_base_of<BasePacket, T>::value, "T must derive from BasePacket");

    std::vector<uint8_t> data = serializePacket(packet);
    ENetPacket* enetPacket
        = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, enetPacket);
  }

  template <typename T> void Manager::broadcastPacket(const T& packet) {
    static_assert(std::is_base_of<BasePacket, T>::value, "T must derive from BasePacket");

    std::vector<uint8_t> data = serializePacket(packet);
    ENetPacket* enetPacket
        = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);

    if (isServer()) {
      for (ENetPeer* peer : connectedPeers) {
        enet_peer_send(peer, 0, enetPacket);
      }
    } else if (isClient() && serverPeer) {
      enet_peer_send(serverPeer, 0, enetPacket);
    }

    enet_host_flush(isServer() ? server : client);
  }

  template <typename T> void Manager::sendToServer(const T& packet) {
    if (isClient() && serverPeer) {
      sendPacket(packet, serverPeer);
    }
  }

  template <typename T>
  void Manager::registerPacketHandler(std::function<void(const T&, ENetPeer*)> handler) {
    static_assert(std::is_base_of<BasePacket, T>::value, "T must derive from BasePacket");

    PacketType type = T().getType();
    packetHandlers[type] = [handler](const std::vector<uint8_t>& data, ENetPeer* peer) {
      T packet = deserializePacket<T>(data);
      handler(packet, peer);
    };
  }

  template <typename T> std::vector<uint8_t> Manager::serializePacket(const T& packet) {
    std::ostringstream oss;
    {
      cereal::BinaryOutputArchive archive(oss);
      archive(packet.getType(), packet);
    }
    const std::string& str = oss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
  }

  template <typename T> T Manager::deserializePacket(const std::vector<uint8_t>& data) {
    std::istringstream iss(std::string(data.begin(), data.end()));
    cereal::BinaryInputArchive archive(iss);

    PacketType type;
    archive(type);

    T packet;
    archive(packet);
    return packet;
  }

  template <typename T> void Manager::broadcastToOthers(const T& packet, ENetPeer* exclude) {
    static_assert(std::is_base_of<BasePacket, T>::value, "T must derive from BasePacket");

    std::vector<uint8_t> data = serializePacket(packet);
    ENetPacket* enetPacket
        = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);

    spdlog::debug("Broadcasting packet {} to others", packet.toString());

    if (isServer()) {
      for (ENetPeer* peer : connectedPeers) {
        if (peer->connectID != exclude->connectID) {
          enet_peer_send(peer, 0, enetPacket);
        }
      }
      enet_host_flush(server);
    }
  }
}  // namespace swarm::network
