// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Core/network_manager.h"
#include "enet/enet.h"

#include "scoped_array.h"
#include "Core/platform_types.h"

#include <iostream>

namespace Aloe {

  struct NetworkManager::Data {
  };

  struct Server::Data {
    ENetAddress address;
    ENetHost* server;

    u16 peers[MAX_PLAYERS];
    u16 n_connections = 0;
    ENetEvent last_event;
    ENetPacket* last_packet_received;
  };

  struct Client::Data {
    ENetHost* client;
    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer;

    bool connected = false;
    u16 client_id;
    ENetPacket* last_packet_received;
  };

  // Network manager ----------------------------------------------------------------

  NetworkManager::NetworkManager() { 
    data = new Data;
  }

  NetworkManager::~NetworkManager() {
    delete data;
  }

  bool NetworkManager::init() {
    if (enet_initialize() != 0) {
      return false;
    }
    return true;
  }

  // Client -------------------------------------------------------------------------

  Client::Client() {
    data = new Data;
  }

  Client::~Client() {
    enet_host_destroy(data->client);
    delete data;
  }

  bool Client::create() {
    data->client = enet_host_create(NULL ,1 ,2 ,0 ,0);
    if (data->client == NULL) {
      return false;
    }
    return true;
  }

  bool Client::connect(int port, const char* host){
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&data->address, host);
    data->address.port = port;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    data->peer = enet_host_connect(data->client, &data->address, 2, 0);
    if (data->peer == NULL) {
      return false;
    }
    return true;
  }

  bool Client::processEvents(NetworkEvent* event){

    ENetEvent enet_event;
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if(enet_host_service(data->client, &enet_event, 0)) {

      switch (enet_event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        *event = Event_Connect;
        return true;
      case ENET_EVENT_TYPE_RECEIVE:
        *event = Event_PacketReceived;

        data->last_packet_received = enet_event.packet;
        return true;
      }

    }
    return false;
  }

  void Client::getPacketReceived(PacketData* packet) {

    packet->data = (void*)data->last_packet_received->data;
    packet->size = data->last_packet_received->dataLength;

  }

  void Client::deletePacketReceived(){
    enet_packet_destroy(data->last_packet_received);
  }

  void Client::setClientId(u16 id){
    data->client_id = id;
    data->connected = true;
  }

  u16 Client::getClientId(){
    return data->client_id;
  }

  bool Client::isConnected(){
    return data->connected;
  }

  void Client::sendDataPacket(void* src, u16 size){

    ENetPacket* packet = enet_packet_create(
      src,
      size,
      ENET_PACKET_FLAG_RELIABLE);

    // Data packet
    enet_peer_send(data->peer, 0, packet);
  }


  // Server -------------------------------------------------------------------------

  Server::Server() {
    data = new Data;
  }

  Server::~Server() {
    enet_host_destroy(data->server);
    atexit(enet_deinitialize);
    delete data;
  }
  bool Server::create(int port){
    data->address.host = ENET_HOST_ANY;
    data->address.port = port;

    data->server = enet_host_create(&data->address, MAX_PLAYERS, 2, 0, 0);
    if (data->server == NULL) {
      return false;
    }
    return true;
  }

  bool Server::processEvents(NetworkEvent* event){
    
    ENetEvent enet_event;
    /* Wait up to 1000 milliseconds for an event. */

    if(enet_host_service(data->server, &enet_event, 0) > 0){
      data->last_event = enet_event;

      switch (enet_event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        if (data->n_connections >= MAX_PLAYERS) {        
          *event = Event_Connect_Surpass;
          return false;
        }

        *event = Event_Connect;
        data->peers[data->n_connections] = enet_event.peer->incomingPeerID;
        data->n_connections++;

        return true;
      case ENET_EVENT_TYPE_RECEIVE:
        *event = Event_PacketReceived;
      
        data->last_packet_received = enet_event.packet;

        return true;
      case ENET_EVENT_TYPE_DISCONNECT:
        *event = Event_Disconnect;
        /* Reset the peer's client information. */
        enet_event.peer->data = NULL;
        return true;
      }

    }
    return false;
  }
  void Server::getPacketReceived(PacketData* packet){
    packet->data = (void*)data->last_packet_received->data;
    packet->size = data->last_packet_received->dataLength;
    packet->client_id = data->last_event.peer->incomingPeerID;
  }
  void Server::deletePacketReceived(){
    enet_packet_destroy(data->last_packet_received);
  }
  u16 Server::getNumberConnections(){
    return data->n_connections;
  }
  void Server::sendDataPacket(scoped_array<float>* send_data){
   
    ENetPacket* packet = enet_packet_create(
      &send_data->get()[0],
      send_data->sizeInBytes(),
      ENET_PACKET_FLAG_RELIABLE);

    // Send Map data packets to players
    enet_peer_send(&data->server->peers[data->peers[0]], 0, packet);
    enet_peer_send(&data->server->peers[data->peers[1]], 0, packet);
  }

  void Server::sendConnectionPackets() {

    //Send packet to client with the player id

    PacketType t = Packet_Connection;
    u32 conn_data[2];
    conn_data[0] = t;
    conn_data[1] = 0;

    ENetPacket* packet = enet_packet_create(
      conn_data,
      sizeof(conn_data),
      ENET_PACKET_FLAG_RELIABLE);

    // Send first packet
    enet_peer_send(&data->server->peers[data->peers[0]], 0, packet);

    // Send seconds packet
    conn_data[1] = 1;
    packet = enet_packet_create(
      conn_data,
      sizeof(conn_data),
      ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(&data->server->peers[data->peers[1]], 0, packet);
    
  }

  void Server::sendDestroyPackets() {
    //Send packet to client with the player id

    PacketType t = Packet_DestroyLevel;
    float conn_data[2];
    conn_data[0] = t;
    conn_data[1] = 0;

    ENetPacket* packet = enet_packet_create(
      conn_data,
      sizeof(conn_data),
      ENET_PACKET_FLAG_RELIABLE);

    // Send first packet
    enet_peer_send(&data->server->peers[data->peers[0]], 0, packet);

    // Send seconds packet
    conn_data[1] = 1;
    packet = enet_packet_create(
      conn_data,
      sizeof(conn_data),
      ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(&data->server->peers[data->peers[1]], 0, packet);
  }

  void Server::sendPacketTo(void * src, u16 size, u16 dst) {    
    ENetPacket* packet = enet_packet_create(
      src,
      size,
      ENET_PACKET_FLAG_RELIABLE);

    // Data packet
    enet_peer_send(&data->server->peers[data->peers[dst]], 0, packet);
  }

}