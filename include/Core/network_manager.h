// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _NETWORK_MANAGER_H_
#define _NETWORK_MANAGER_H_ 1

#define MAX_PLAYERS 2

#include "Game/moving_cube.h"
#include "ECS/ecs_manager.h"
#include "ECS/Components/transform_component.h"
#include "Core/platform_types.h"
#include "glm.hpp"
#include "scoped_array.h"

namespace Aloe {

  enum NetworkEvent {
    Event_None,
    Event_Connect,
    Event_Connect_Surpass,
    Event_PacketReceived,
    Event_Disconnect,

  };

  enum PacketType {
    Packet_Connection,
    Packet_CreateMap,
    Packet_CreateMapDynamics,
    Packet_UpdateMapDynamics,
    Packet_PlayerData,
    Packet_InputPacket,
    Packet_DestroyLevel,
    Packet_MapLoaded
  };

  struct PacketData {
    PacketType type;
    u16 client_id;
    void* data;
    u16 size;
  };

  // Network manager ----------------------------------------------------------------

  class NetworkManager {

  public:

    NetworkManager();
    ~NetworkManager();

    /** Network init, must be called before doing any Client/Server action*/
    bool init();

  private:

    //Internat data
    struct Data;
    Data* data;
  };

  // Client -------------------------------------------------------------------------

  class Client {
  public:

    Client();
    ~Client();

    /** Create the client*/
    bool create();
    /** Connects to a host*/
    bool connect(int port,const char* host);
    /** Process the events and outputs the current event*/
    bool processEvents(NetworkEvent* event);
    /** Gets the last received packet*/
    void getPacketReceived(PacketData* packet);
    /** Deletes the last received packet*/
    void deletePacketReceived();

    /** Sets client ID*/
    void setClientId(u16 id);
    /** Gets the client ID*/
    u16 getClientId();
    /** Get if client is connected*/
    bool isConnected();

    /** Send a packet of data*/
    void sendDataPacket(void* src, u16 size);

  private:

    //Internal data
    struct Data;
    Data* data;
  };

  // Server -------------------------------------------------------------------------

  class Server {
  public:

    Server();
    ~Server();

    /** Creates the server with "port"*/
    bool create(int port);
    /** Process events and returns the event received*/
    bool processEvents(NetworkEvent *event);
    /** Gets the last received packet*/
    void getPacketReceived(PacketData* packet);
    /** Delete the last received packet*/
    void deletePacketReceived();

    /**  Get the number of clients connected*/
    u16 getNumberConnections();

    /** Sends data packet to both clients*/
    void sendDataPacket(scoped_array<float>* data);
    /** Sends a connection packet to both clients*/
    void sendConnectionPackets();
    /** Send a packet to a client*/
    void sendPacketTo(void* src,u16 size,u16 dst);
    /** Send a destroy packet to clients*/
    void sendDestroyPackets();

  private:

    //Internat data
    struct Data;
    Data* data;
  };

}

#endif