// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "user_main.h"

#include "ECS/ecs_manager.h"
#include "ECS/Systems/input_system.h"
#include "ECS/Components/input_component.h"
#include "ECS/Components/transform_component.h"
#include "ECS/Components/camera_component.h"
#include "ECS/Components/rigid_body.h"
#include "ECS/Components/render_component.h"

#include "Core/engine.h"
#include "Core/timer.h"
#include "Core/audio_player.h"

#include "Graphics/shader.h"
#include "Graphics/Materials/color_material.h"
#include "Graphics/Materials/grass_material.h"
#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include "Graphics/skybox.h"

#include "Core/network_manager.h"

#include "px_mem.h"

#include "gtc/matrix_transform.hpp"
#include <chrono>

#define CAMERA_SPEED 0.5f

namespace Aloe {

  ECSManager* ecs;
  Mesh* cube_mesh;

  //TEMPORAL CAMERA MOVEMENT
  Entity camera_entity;
  void MoveCameraAround(double xpos, double ypos) {

    static float lastX = 400;
    static float lastY = 300;
    static bool firstMouse = true;
    static float yaw = -90.0f;
    static float pitch = 0.0f;

    float c_sensitivity = 0.1f;

    ECSManager* ecs = GetGlobalECS();
    CameraComponent& c = ecs->GetComponent<CameraComponent>(camera_entity);

    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= c_sensitivity;
    yoffset *= c_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)pitch = 89.0f;
    if (pitch < -89.0f)pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    c.front = glm::normalize(direction);
  }

  struct InputPacket {
    float x;
    float y;
  };

  InputPacket input_packet;
  void MoveCameraRight() { 
    input_packet.x = 1.0f;
  }
  void MoveCameraLeft() {
    input_packet.x = -1.0f;
  }
  void MoveCameraUp() {
    input_packet.y = 1.0f;
  }
  void MoveCameraDown() {
    input_packet.y = -1.0f;
  }

  void CameraSetup() {

    // Camera
    Entity input_entity;
    input_entity = ecs->CreateEntity();

    //Input Component
    InputComponent inputComp;
    AddInputBinding(inputComp, 'W', MoveCameraUp);
    AddInputBinding(inputComp, 'A', MoveCameraLeft);
    AddInputBinding(inputComp, 'S', MoveCameraDown);
    AddInputBinding(inputComp, 'D', MoveCameraRight);
    AddMouseInputBinding(inputComp, MoveCameraAround);
    ecs->AddComponent(input_entity, inputComp);

    // Camera
    camera_entity = ecs->CreateEntity();

    CameraComponent c;

    c.setViewMatrix(
      glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(0.f, 0.f, -1.f),
      glm::vec3(0.f, 1.f, 0.f)).
      setProjectionMatrix(90.f, 0.1f, 1000.0f, 900, 600);

    ecs->AddComponent(camera_entity, c);
    SetMainCamera(camera_entity);
  }

  Skybox skybox;
  void SetupSkybox() {
    std::vector<std::string> cubemap_paths;
    cubemap_paths.push_back("../../resources/cubemaps/clouds/back.jpg");
    cubemap_paths.push_back("../../resources/cubemaps/clouds/front.jpg");
    cubemap_paths.push_back("../../resources/cubemaps/clouds/up.jpg");
    cubemap_paths.push_back("../../resources/cubemaps/clouds/down.jpg");
    cubemap_paths.push_back("../../resources/cubemaps/clouds/left.jpg");
    cubemap_paths.push_back("../../resources/cubemaps/clouds/right.jpg");

    skybox.create(cubemap_paths);
    skybox.setAsCurrent();
  }

  ColorMaterial* white_material;
  ColorMaterial* yellow_material;
  ColorMaterial* red_material;
  ColorMaterial* blue_material;
  ColorMaterial* brown_material;
  Entity player2_entity;
  void Player2Setup() {
  
    player2_entity = ecs->CreateEntity();

    TransformComponent tr;
    tr.position = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = glm::vec3(1.0f,2.0,1.0f);

    RenderComponent re;
    re.mesh = cube_mesh;
    re.material = yellow_material;

    ecs->AddComponent<TransformComponent>(player2_entity, tr);
    ecs->AddComponent<RenderComponent>(player2_entity, re);
  }

  NetworkManager net_manager;
  Client client;

  //Map
   //Dynamic map
  std::vector<Entity> map_dynamic;
  std::vector<Entity> map_static;

  void DestroyLevel() {
  
    //Destroy Level
    for (int i = 0; i < map_dynamic.size(); ++i) {
      ecs->DestroyEntity(map_dynamic[i]);
    }
    for (int i = 0; i < map_static.size(); ++i) {
      ecs->DestroyEntity(map_static[i]);
    }

    map_dynamic.clear();
    map_static.clear();

  }

  void MapSetup(float* data, int size) {

    RenderComponent re;
    re.mesh = cube_mesh;

    for (int i = 0; i < size * 13; i += 13) {
      TransformComponent tr;

      tr.position = glm::vec3(data[i], data[i + 1], data[i + 2]);
      tr.rotation = glm::vec3(data[i + 3], data[i + 4], data[i + 5]);
      tr.scale = glm::vec3(data[i + 6], data[i + 7], data[i + 8]);

      glm::vec4 color = glm::vec4(data[i + 9], data[i + 10], data[i + 11], data[i + 12]);

      re.material = white_material;
      if (color == glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))re.material = blue_material;
      if (color == glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))re.material = red_material;
      if (color == glm::vec4(0.5f, 0.35f, 0.05f, 1.0f))re.material = brown_material;

      Entity entity = ecs->CreateEntity();
      map_static.push_back(entity);

      ecs->AddComponent<TransformComponent>(entity, tr);
      ecs->AddComponent<RenderComponent>(entity, re);
    }
  }

  void DynamicMapSetup(float* data,int size) {

    RenderComponent re;
    re.mesh = cube_mesh;

    for (int i = 0; i < size * 13;i+=13) {
      TransformComponent tr;

      tr.position = glm::vec3(data[i], data[i + 1], data[i + 2]);
      tr.rotation = glm::vec3(data[i + 3], data[i + 4], data[i + 5]);
      tr.scale = glm::vec3(data[i + 6], data[i + 7], data[i + 8]);

      glm::vec4 color = glm::vec4(data[i + 9], data[i + 10], data[i + 11], data[i + 12]);
      
      if (color == glm::vec4(0.0f,0.0f,1.0f,1.0f))re.material = blue_material;
      if (color == glm::vec4(1.0f,0.0f,0.0f,1.0f))re.material = red_material;
      if (color == glm::vec4(0.5f, 0.35f, 0.05f, 1.0f))re.material = brown_material;

      Entity entity = ecs->CreateEntity();
      map_dynamic.push_back(entity);

      ecs->AddComponent<TransformComponent>(entity, tr);
      ecs->AddComponent<RenderComponent>(entity, re);
    }

  }

  void UpdateMap(float* data, int size) {
    for (int i = 0; i < size * 9; i += 9) {

      TransformComponent& tr = ecs->GetComponent<TransformComponent>(map_dynamic[i / 9]);

      tr.position = glm::vec3(data[i], data[i + 1], data[i + 2]);
      tr.rotation = glm::vec3(data[i + 3], data[i + 4], data[i + 5]);
      tr.scale = glm::vec3(data[i + 6], data[i + 7], data[i + 8]);
    }
  }



  AudioPlayer audio_player;
  void Aloe::Start() {

    CreateWindowContext(900, 600, "Aloe Client");

    ecs = GetGlobalECS();
    cube_mesh = CreateCubeMesh();

    white_material = CreateColorMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    yellow_material = CreateColorMaterial(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    red_material = CreateColorMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    blue_material = CreateColorMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    brown_material = CreateColorMaterial(glm::vec4(0.5f, 0.35f, 0.05f, 1.0f));

    CameraSetup();
    SetupSkybox();

    net_manager.init();
    client.create();

    audio_player.LoadClip("../../resources/audio/guitarmass.ogg");

    if (!client.connect(1234, "localhost")) {
      printf("\nNot connected");
    }

  }
  void Aloe::Update() {

    NetworkEvent event;

    while (client.processEvents(&event)) {
      switch (event) {
      case Event_Connect:
        printf("\n[NETWORK] Connected to server");
        break;
      case Event_PacketReceived:

        //Get Packet
        PacketData packet;
        PacketType t;
        float* tr_data;
        client.getPacketReceived(&packet);
        t = (PacketType)((float*)packet.data)[0];

        //Check packet type
        switch (t) {
        case Packet_Connection:
          printf("\n[NETWORK] 2 players ready");
          printf("\n[NETWORK] Client id: %d", ((u32*)packet.data)[1]);
          client.setClientId(((u32*)packet.data)[1]);

          audio_player.PlaySound();

          Player2Setup();
          break;
        case Packet_CreateMap:
          printf("\n[NETWORK] Setup map");
          tr_data = ((float*)(packet.data) + 1);
          MapSetup(tr_data, (packet.size - 4) / (13 * 4));
          break;
        case Packet_CreateMapDynamics:
        {
          printf("\n[NETWORK] Setup map dynamics");
          tr_data = ((float*)(packet.data) + 1);
          DynamicMapSetup(tr_data, (packet.size - 4) / (13 * 4));

          //Send packet saying loading was completed
          float data[2] = { Packet_MapLoaded,client.getClientId() };
          client.sendDataPacket(data, sizeof(data));
        }
          break;
        case Packet_PlayerData:
        {
          TransformComponent& tr = ecs->GetComponent<TransformComponent>(player2_entity);
          CameraComponent& c = ecs->GetComponent<CameraComponent>(camera_entity);

          // Set other play position
          float* f_data;
          f_data = (float*)packet.data;
          
          // Set my position          
          if (client.getClientId() == 0) {
            c.position.x = f_data[1];
            c.position.y = f_data[2];
            c.position.z = f_data[3];   

            tr.position.x = f_data[4];
            tr.position.y = f_data[5];
            tr.position.z = f_data[6];  
          }else {
            tr.position.x = f_data[1];
            tr.position.y = f_data[2];
            tr.position.z = f_data[3];
            
            c.position.x = f_data[4];
            c.position.y = f_data[5];
            c.position.z = f_data[6];
          }
        }
          break;
        case Packet_UpdateMapDynamics:
        {
          tr_data = ((float*)(packet.data) + 1);
          UpdateMap(tr_data, (packet.size - 4) / (9 * 4));
        }
          break;
        case Packet_DestroyLevel:
          printf("\n[NETWORK] Destroy Level");
          DestroyLevel();
          break;
        }

        client.deletePacketReceived();

        break;
      }
    }
    
    if (client.isConnected()) {
      CameraComponent& c = ecs->GetComponent<CameraComponent>(camera_entity);
      // Send my data to server     
      float data[6] = {Packet_PlayerData,c.front.x,c.front.y,c.front.z,
        input_packet.x,input_packet.y }; 
      client.sendDataPacket(data,sizeof(data));
      
      // Reset input to avoid going forward continuously
      input_packet.x = 0.0f;
      input_packet.y = 0.0f;
    }
  }

  void Aloe::Close() {
  }
}