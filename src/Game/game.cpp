// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Game/game.h"

#define CAMERA_SPEED 0.05f

namespace Aloe {

  void MyEventListerner::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) {

    // Trigger events
    for (int i = 0; i < callbackData.getNbOverlappingPairs(); ++i) {
      reactphysics3d::OverlapCallback::OverlapPair call_data = callbackData.getOverlappingPair(i);

      // Cubes 
      for (int c = 0; c < game->moving_cubes.size(); ++c) {

        RigidBodyComponent c_rb = game->ecs->GetComponent<RigidBodyComponent>(game->moving_cubes[c].entity);

        // Check if the collision was between a moving cube ("trigger" collider is the number 1)
        if (call_data.getCollider1() == c_rb.rigidBody->getCollider(1) || call_data.getCollider2() == c_rb.rigidBody->getCollider(1)) {

        // Players
          for (int p = 0; p < MAX_PLAYERS; ++p) {

            RigidBodyComponent p_rb = game->ecs->GetComponent<RigidBodyComponent>(game->players[p]);

            // Check if the collision was between the player and the cubes ("player only has 1 collider") 
            if (call_data.getCollider1() == p_rb.rigidBody->getCollider(0) || call_data.getCollider2() == p_rb.rigidBody->getCollider(0)) {

              Entity player = game->players[p];
              TransformComponent* p_tr = &game->ecs->GetComponent<TransformComponent>(player);

              game->moving_cubes[c].playerCollision(p_tr);

              printf("\n Collision between player and moving cube");

            }
          }
        }
      }
    }
  }

  reactphysics3d::decimal MyRaycastListener::notifyRaycastHit(const reactphysics3d::RaycastInfo& raycastInfo) {  

    cube->frontCollision();
    return 0.0f;
  }

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
  void MoveCameraRight() {

    Engine* e = Engine::get();
    CameraComponent& c = e->ecs->GetComponent<CameraComponent>(e->main_camera);
    c.position += glm::normalize(glm::cross(c.front, c.up)) * CAMERA_SPEED;
  }
  void MoveCameraLeft() {
    Engine* e = Engine::get();
    CameraComponent& c = e->ecs->GetComponent<CameraComponent>(e->main_camera);
    c.position -= glm::normalize(glm::cross(c.front, c.up)) * CAMERA_SPEED;
  }
  void MoveCameraUp() {

    Engine* e = Engine::get();
    CameraComponent& c = e->ecs->GetComponent<CameraComponent>(e->main_camera);
    c.position += CAMERA_SPEED * c.front;
  }
  void MoveCameraDown() {

    Engine* e = Engine::get();
    CameraComponent& c = e->ecs->GetComponent<CameraComponent>(e->main_camera);
    c.position -= CAMERA_SPEED * c.front;
  }

  //Network
  void Game::CreateMapDataPacket(){

    PacketType t = Packet_CreateMap;

    create_map_packet.alloc(1 + (map_static_trs.size())  * 13);
    create_map_packet[0] = t;

    for (int i = 0; i < map_static_trs.size();i++) {
      create_map_packet[i * 13 + 1] = map_static_trs[i]->position.x;
      create_map_packet[i * 13 + 2] = map_static_trs[i]->position.y;
      create_map_packet[i * 13 + 3] = map_static_trs[i]->position.z;

      create_map_packet[i * 13 + 4] = map_static_trs[i]->rotation.x;
      create_map_packet[i * 13 + 5] = map_static_trs[i]->rotation.y;
      create_map_packet[i * 13 + 6] = map_static_trs[i]->rotation.z;

      create_map_packet[i * 13 + 7] = map_static_trs[i]->scale.x;
      create_map_packet[i * 13 + 8] = map_static_trs[i]->scale.y;
      create_map_packet[i * 13 + 9] = map_static_trs[i]->scale.z;

      // Color
      ColorMaterial* color_mat = (ColorMaterial*)ecs->GetComponent<RenderComponent>(map_static[i]).material;
      create_map_packet[i * 13 + 10] = color_mat->color.r;
      create_map_packet[i * 13 + 11] = color_mat->color.g;
      create_map_packet[i * 13 + 12] = color_mat->color.b;
      create_map_packet[i * 13 + 13] = color_mat->color.a;
    }
  }
  void Game::CreateDynamicsDataPacket() {
    PacketType t = Packet_CreateMapDynamics;

    create_map_dynamics_packet.alloc(1 + (map_dynamic_trs.size()) * 13);
    create_map_dynamics_packet[0] = t;

    for (int i = 0; i < map_dynamic_trs.size(); i++) {
      create_map_dynamics_packet[i * 13 + 1] = map_dynamic_trs[i]->position.x;
      create_map_dynamics_packet[i * 13 + 2] = map_dynamic_trs[i]->position.y;
      create_map_dynamics_packet[i * 13 + 3] = map_dynamic_trs[i]->position.z;

      create_map_dynamics_packet[i * 13 + 4] = map_dynamic_trs[i]->rotation.x;
      create_map_dynamics_packet[i * 13 + 5] = map_dynamic_trs[i]->rotation.y;
      create_map_dynamics_packet[i * 13 + 6] = map_dynamic_trs[i]->rotation.z;

      create_map_dynamics_packet[i * 13 + 7] = map_dynamic_trs[i]->scale.x;
      create_map_dynamics_packet[i * 13 + 8] = map_dynamic_trs[i]->scale.y;
      create_map_dynamics_packet[i * 13 + 9] = map_dynamic_trs[i]->scale.z;

      // Color
      ColorMaterial* color_mat = (ColorMaterial*)ecs->GetComponent<RenderComponent>(map_dynamic[i]).material;
      create_map_dynamics_packet[i * 13 + 10] = color_mat->color.r;
      create_map_dynamics_packet[i * 13 + 11] = color_mat->color.g;
      create_map_dynamics_packet[i * 13 + 12] = color_mat->color.b;
      create_map_dynamics_packet[i * 13 + 13] = color_mat->color.a;
    }

  }
  void Game::CreateUpdateDynamicsDataPacket() {
    PacketType t = Packet_UpdateMapDynamics;

    update_map_dynamics_packet.alloc(1 + (map_dynamic_trs.size()) * 9);
    update_map_dynamics_packet[0] = t;

    for (int i = 0; i < map_dynamic_trs.size(); i++) {
      update_map_dynamics_packet[i * 9 + 1] = map_dynamic_trs[i]->position.x;
      update_map_dynamics_packet[i * 9 + 2] = map_dynamic_trs[i]->position.y;
      update_map_dynamics_packet[i * 9 + 3] = map_dynamic_trs[i]->position.z;

      update_map_dynamics_packet[i * 9 + 4] = map_dynamic_trs[i]->rotation.x;
      update_map_dynamics_packet[i * 9 + 5] = map_dynamic_trs[i]->rotation.y;
      update_map_dynamics_packet[i * 9 + 6] = map_dynamic_trs[i]->rotation.z;

      update_map_dynamics_packet[i * 9 + 7] = map_dynamic_trs[i]->scale.x;
      update_map_dynamics_packet[i * 9 + 8] = map_dynamic_trs[i]->scale.y;
      update_map_dynamics_packet[i * 9 + 9] = map_dynamic_trs[i]->scale.z;
    }
  }
  void Game::UpdateDynamicsDataPacket() {

    for (int i = 0; i < map_dynamic_trs.size(); i++) {
      update_map_dynamics_packet[i * 9 + 1] = map_dynamic_trs[i]->position.x;
      update_map_dynamics_packet[i * 9 + 2] = map_dynamic_trs[i]->position.y;
      update_map_dynamics_packet[i * 9 + 3] = map_dynamic_trs[i]->position.z;

      update_map_dynamics_packet[i * 9 + 4] = map_dynamic_trs[i]->rotation.x;
      update_map_dynamics_packet[i * 9 + 5] = map_dynamic_trs[i]->rotation.y;
      update_map_dynamics_packet[i * 9 + 6] = map_dynamic_trs[i]->rotation.z;

      update_map_dynamics_packet[i * 9 + 7] = map_dynamic_trs[i]->scale.x;
      update_map_dynamics_packet[i * 9 + 8] = map_dynamic_trs[i]->scale.y;
      update_map_dynamics_packet[i * 9 + 9] = map_dynamic_trs[i]->scale.z;
    }
  }

  //Create functions
  Entity Game::CreateStaticCube(glm::vec3 pos, glm::vec3 scale, MaterialColor col) {
    RenderComponent re;
    re.mesh = cube_mesh;
    re.material = materials[col];

    TransformComponent tr;
    tr.position = pos;
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = scale;

    //Rigid body
    RigidBodyComponent rb;
    rb.create(tr);
    reactphysics3d::BoxShape* box;
    box = Engine::get()->physicsCommon.createBoxShape(reactphysics3d::Vector3(
      tr.scale.x * 0.5f,
      tr.scale.y * 0.5f,
      tr.scale.z * 0.5f));
    rb.rigidBody->addCollider(box, reactphysics3d::Transform::identity());
    rb.setType(BodyType::STATIC);

    Entity entity = ecs->CreateEntity();

    ecs->AddComponent<TransformComponent>(entity, tr);
    ecs->AddComponent<RenderComponent>(entity, re);
    ecs->AddComponent<RigidBodyComponent>(entity, rb);
   
    map_static.push_back(entity);
    map_static_trs.push_back(&ecs->GetComponent<TransformComponent>(entity));

    return map_static.size() -1;
  }
  u32 Game::CreateMovingCube(glm::vec3 pos, MaterialColor col) {
  
    RenderComponent re;
    re.mesh = cube_mesh;
    re.material = materials[col];

    TransformComponent tr;

    tr.position = pos;
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = glm::vec3(1.0f, 1.0, 1.0f);

    Entity entity = ecs->CreateEntity();

    //Rigid body
    RigidBodyComponent rb;
    rb.create(tr);
    reactphysics3d::BoxShape* box;
    reactphysics3d::BoxShape* trigger;

    box = Engine::get()->physicsCommon.createBoxShape(reactphysics3d::Vector3(
      tr.scale.x * 0.5f,
      tr.scale.y * 0.5f,
      tr.scale.z * 0.5f));
    trigger = Engine::get()->physicsCommon.createBoxShape(reactphysics3d::Vector3(
      tr.scale.x * 0.5f + 0.01f,
      tr.scale.y * 0.5f + 0.01f,
      tr.scale.z * 0.5f + 0.01f));

    rb.rigidBody->addCollider(box, reactphysics3d::Transform::identity());
    rb.rigidBody->addCollider(trigger, reactphysics3d::Transform::identity());
    rb.rigidBody->getCollider(1)->setIsTrigger(true);
    rb.setType(BodyType::DYNAMIC);

    ecs->AddComponent<TransformComponent>(entity, tr);
    ecs->AddComponent<RenderComponent>(entity, re);
    ecs->AddComponent<RigidBodyComponent>(entity, rb);

    //Store moving cube
    MovingCube cube;
    if (col == Color_Red)cube.type = Red_Cube;
    if (col == Color_Blue)cube.type = Blue_Cube;
    map_dynamic.push_back(entity);
    cube.entity = entity;
    cube.tr = &ecs->GetComponent<TransformComponent>(entity);
    cube.rd = &ecs->GetComponent<RenderComponent>(entity);
    moving_cubes.push_back(cube);
    map_dynamic_trs.push_back(cube.tr);

    return moving_cubes.size() - 1;
  }
  u32 Game::CreateDoor(glm::vec3 pos, glm::vec3 scale) {

    RenderComponent re;
    re.mesh = cube_mesh;
    re.material = materials[Color_Brown];

    TransformComponent tr;
    tr.position = pos;
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = scale;

    //Rigid body
    RigidBodyComponent rb;
    rb.create(tr);
    reactphysics3d::BoxShape* box;
    box = Engine::get()->physicsCommon.createBoxShape(reactphysics3d::Vector3(
      tr.scale.x * 0.5f,
      tr.scale.y * 0.5f,
      tr.scale.z * 0.5f));
    rb.rigidBody->addCollider(box, reactphysics3d::Transform::identity());
    rb.setType(BodyType::STATIC);

    Entity entity = ecs->CreateEntity();

    ecs->AddComponent<TransformComponent>(entity, tr);
    ecs->AddComponent<RenderComponent>(entity, re);
    ecs->AddComponent<RigidBodyComponent>(entity, rb);

    //Store door
    Door door;
    map_dynamic.push_back(entity);
    door.tr = &ecs->GetComponent<TransformComponent>(entity);
    door.game = this;
    map_dynamic_trs.push_back(door.tr);
    doors.push_back(door);

    return doors.size() - 1;
  }
  u32 Game::CreateDetector(glm::vec3 pos, glm::vec3 scale, MaterialColor col) {
    RenderComponent re;
    re.mesh = cube_mesh;
    re.material = materials[col];

    TransformComponent tr;
    tr.position = pos;
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = scale;

    //Rigid body
    RigidBodyComponent rb;
    rb.create(tr);
    reactphysics3d::BoxShape* box;
    box = Engine::get()->physicsCommon.createBoxShape(reactphysics3d::Vector3(
      tr.scale.x * 0.5f,
      tr.scale.y * 0.5f,
      tr.scale.z * 0.5f));
    rb.rigidBody->addCollider(box, reactphysics3d::Transform::identity());
    rb.setType(BodyType::STATIC);

    Entity entity = ecs->CreateEntity();

    ecs->AddComponent<TransformComponent>(entity, tr);
    ecs->AddComponent<RenderComponent>(entity, re);
    ecs->AddComponent<RigidBodyComponent>(entity, rb);

    DetectorCube detector;
    if (col == Color_Red)detector.type = Red_Cube;
    if (col == Color_Blue)detector.type = Blue_Cube;
    detector.entity = entity;
    detector.tr = &ecs->GetComponent<TransformComponent>(entity);
    detector.rd = &ecs->GetComponent<RenderComponent>(entity);
    detector.game = this;

    map_static.push_back(entity);
    map_static_trs.push_back(detector.tr);
    detectors.push_back(detector);

    return detectors.size() -1;
  }

  Game* game;
  Game::Game() {
    game = this;
  }
  Game::~Game() {}
  void Game::CameraSetup() {

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

  void Game::PlayersSetup() {

    RenderComponent re;
    re.mesh = CreateCubeMesh();
    re.material = materials[Color_Yellow];

    for (int i = 0; i < MAX_PLAYERS; ++i) {

      TransformComponent tr;

      tr.position = glm::vec3(-8.0f + 16.0f * i, 2.0f, -5.0f);
      tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
      tr.scale = glm::vec3(1.0f, 2.0, 1.0f);

      //Rigid body
      RigidBodyComponent rb;
      rb.create(tr);
      reactphysics3d::BoxShape* box;
      box = Engine::get()->physicsCommon.createBoxShape(reactphysics3d::Vector3(
        tr.scale.x * 0.5f,
        tr.scale.y * 0.5f,
        tr.scale.z * 0.5f));

      rb.rigidBody->addCollider(box, reactphysics3d::Transform::identity());
      rb.setType(BodyType::DYNAMIC);

      players[i] = ecs->CreateEntity();

      ecs->AddComponent<TransformComponent>(players[i], tr);
      ecs->AddComponent<RenderComponent>(players[i], re);
      ecs->AddComponent<RigidBodyComponent>(players[i], rb);
    }
  }

  void Game::PlayerCameraMovement(int client, float* front, float* input) {

    TransformComponent& tr = ecs->GetComponent<TransformComponent>(players[client]);

    float y = tr.position.y;

    Engine* e = Engine::get();
    // X
    tr.position += (input[0]) * (glm::normalize(glm::cross(
      glm::vec3(front[0], front[1], front[2]),
      glm::vec3(0.0f, 1.0f, 0.0f))) * CAMERA_SPEED);
    // Y
    tr.position += (input[1]) * (CAMERA_SPEED * glm::vec3(front[0], front[1], front[2]));

    // Get back y position in order to now move up or down
    tr.position.y = y;
  }

  void Game::DestroyLevel() {
    //Destroy level
    for (int i = 0; i < map_static.size(); ++i) {
      ecs->GetComponent<RigidBodyComponent>(map_static[i]).destroyBody();
      ecs->DestroyEntity(map_static[i]);
    }
    for (int i = 0; i < map_dynamic.size(); ++i) {
      ecs->GetComponent<RigidBodyComponent>(map_dynamic[i]).destroyBody();
      ecs->DestroyEntity(map_dynamic[i]);
    }

    map_static_trs.clear();
    map_dynamic_trs.clear();
    map_static.clear();
    map_dynamic.clear();
    moving_cubes.clear();
    detectors.clear();
    doors.clear();
  }

  void Game::Start() {
    ecs = GetGlobalECS();

    event_listener.game = this;
    Engine::get()->physicsWorld->setEventListener(&event_listener);
    materials[Color_White] = CreateColorMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    materials[Color_Yellow] = CreateColorMaterial(glm::vec4(1.0f,1.0f,0.0f,1.0f));
    materials[Color_Red] = CreateColorMaterial(glm::vec4(1.0f,0.0f,0.0f,1.0f));
    materials[Color_Blue] = CreateColorMaterial(glm::vec4(0.0f,0.0f,1.0f,1.0f));
    materials[Color_Brown] = CreateColorMaterial(glm::vec4(0.5f, 0.35f, 0.05f,1.0f));
    cube_mesh = CreateCubeMesh();

    CameraSetup();
    PlayersSetup();

    std::vector<LuaMethod> methods = {
      {lua_CreateStaticBlock,"CreateStaticCube"},
      {lua_CreateDetectorBlock,"CreateDetector"},
      {lua_CreateMovingBlock,"CreateMovingCube"},
      {lua_CreateDoorBlock,"CreateDoor"},
      {lua_SetDoorMovingBlocks,"SetDoorMovingBlocks"},
      {lua_SetPlayerPositions,"SetPlayerPositions"}
    };

    script_manager.create(methods);
    script_manager.setScript("../../src/Game/Scripts/levels.lua");
    script_manager.callFunction("LoadLevel1");

    net_manager.init();
     
    server.create(1234);
  }

  void Game::GameplayElementsUpdate() {
  
    for (int i = 0; i < moving_cubes.size();++i) {
      moving_cubes[i].update();
    }

    for (int i = 0; i < detectors.size(); ++i) {
      detectors[i].update();
    }

    for (int i = 0; i < doors.size(); ++i) {
      doors[i].update();
    }

    for (int i = 0; i < MAX_PLAYERS;++i) {
      if (ecs->GetComponent<TransformComponent>(players[i]).position.z < -17.0f) {
        //Level pass
        if (level == 1) {
          
          DestroyLevel();
          script_manager.callFunction("LoadLevel2");

          //Send packets to clients

          server.sendDestroyPackets();

          CreateMapDataPacket();
          CreateDynamicsDataPacket();
          CreateUpdateDynamicsDataPacket();

          server.sendDataPacket(&create_map_packet);
          server.sendDataPacket(&create_map_dynamics_packet);

          level++;

          break;
        }
      }
    }
  }

  void Game::NetworkUpdate() {
    NetworkEvent event;

    while (server.processEvents(&event)) {
      switch (event) {

      case Event_Connect:
        printf("\n[NETWORK] New connection");
        if (server.getNumberConnections() == MAX_PLAYERS) {
          server.sendConnectionPackets();
          //Send packet with map information

          CreateMapDataPacket();
          CreateDynamicsDataPacket();
          CreateUpdateDynamicsDataPacket();

          server.sendDataPacket(&create_map_packet);
          server.sendDataPacket(&create_map_dynamics_packet);
        }
        break;
      case Event_PacketReceived:
        //Get Packet
        PacketData packet;
        PacketType t;
        server.getPacketReceived(&packet);
        float* f_data;

        f_data = (float*)packet.data;
        t = (PacketType)f_data[0];

        switch (t) {
        case Packet_Connection:
          break;
        case Packet_PlayerData:
        {
          //Update player
          PlayerCameraMovement(packet.client_id, &f_data[1], &f_data[4]);

          // Send updated player position to both players
          TransformComponent& tr1 = ecs->GetComponent<TransformComponent>(players[0]);
          TransformComponent& tr2 = ecs->GetComponent<TransformComponent>(players[1]);
          float p_positions[7] = { Packet_PlayerData,
            tr1.position.x,tr1.position.y,tr1.position.z,
            tr2.position.x,tr2.position.y,tr2.position.z };

          server.sendPacketTo(p_positions, sizeof(p_positions), 0);
          server.sendPacketTo(p_positions, sizeof(p_positions), 1);
        }
         break;
        case Packet_MapLoaded:
          map_loaded[packet.client_id] = true;
          break;
        }

        server.deletePacketReceived();

        break;
      case Event_Disconnect:
        printf("\n[NETWORK] Client disconnection");
        break;
      }
    }

    if (server.getNumberConnections() >= MAX_PLAYERS && map_loaded[0] && map_loaded[1]) {
      UpdateDynamicsDataPacket();
      server.sendDataPacket(&update_map_dynamics_packet);
    }

  }

  void Game::Update() {
    
    NetworkUpdate();
    GameplayElementsUpdate();

  }

  // LUA -----------------------------------------------------------------------------------------------

  int Game::lua_CreateStaticBlock(lua_State* L) {

    /* get number of arguments */
    int n = lua_gettop(L);
    if (n != 7)printf("\n[LUA] Error: wrong number of arguments for CreateStaticBlock");

    glm::vec3 pos = { lua_tonumber(L, 1) ,lua_tonumber(L, 2) ,lua_tonumber(L, 3) };
    glm::vec3 scale = { lua_tonumber(L, 4) ,lua_tonumber(L, 5) ,lua_tonumber(L, 6) };

    int col = lua_tonumber(L, 7);

    Entity en = game->CreateStaticCube(pos,scale,(MaterialColor)col);

    /* return the number of results */

    lua_pushnumber(L, en);

    return 1;
  }
  int Game::lua_CreateDetectorBlock(lua_State* L) {

    /* get number of arguments */
    int n = lua_gettop(L);
    if (n != 7)printf("\n[LUA] Error: wrong number of arguments for lua_CreateDetectorBlock");

    glm::vec3 pos = { lua_tonumber(L, 1) ,lua_tonumber(L, 2) ,lua_tonumber(L, 3) };
    glm::vec3 scale = { lua_tonumber(L, 4) ,lua_tonumber(L, 5) ,lua_tonumber(L, 6) };

    int col = lua_tonumber(L, 7);

    u32 index = game->CreateDetector(pos, scale, (MaterialColor)col);

    /* return the number of results */

    lua_pushnumber(L, index);

    return 1;
  }
  int Game::lua_CreateMovingBlock(lua_State* L) {

    /* get number of arguments */
    int n = lua_gettop(L);
    if (n != 4)printf("\n[LUA] Error: wrong number of arguments for lua_CreateMovingBlock");

    glm::vec3 pos = { lua_tonumber(L, 1) ,lua_tonumber(L, 2) ,lua_tonumber(L, 3) };
    int col = lua_tonumber(L, 4);

    u32 index = game->CreateMovingCube(pos, (MaterialColor)col);

    /* return the number of results */

    lua_pushnumber(L, index);

    return 1;
  }
  int Game::lua_CreateDoorBlock(lua_State* L) {

    /* get number of arguments */
    int n = lua_gettop(L);
    if (n != 6)printf("\n[LUA] Error: wrong number of arguments for lua_CreateDoorBlock");

    glm::vec3 pos = { lua_tonumber(L, 1) ,lua_tonumber(L, 2) ,lua_tonumber(L, 3) };
    glm::vec3 scale = { lua_tonumber(L, 4) ,lua_tonumber(L, 5) ,lua_tonumber(L, 6) };

    u32 index = game->CreateDoor(pos, scale);

    /* return the number of results */

    lua_pushnumber(L, index);

    return 1;
  }
  int Game::lua_SetDoorMovingBlocks(lua_State* L) {
  
    /* get number of arguments */
    int n = lua_gettop(L);

    std::vector<u32> cubes;

    for (int i = 2; i <= n;++i) {
      cubes.push_back(lua_tonumber(L,i));
    }

    game->doors[lua_tonumber(L, 1)].setCubes(cubes);
    
    return 0;
  }

  int Game::lua_SetPlayerPositions(lua_State* L) {

    /* get number of arguments */
    int n = lua_gettop(L);

    game->ecs->GetComponent<TransformComponent>(game->players[0]).position = 
      glm::vec3(lua_tonumber(L,1), lua_tonumber(L, 2), lua_tonumber(L, 3));

    game->ecs->GetComponent<TransformComponent>(game->players[1]).position =
      glm::vec3(lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));

    return 0;
  }
}