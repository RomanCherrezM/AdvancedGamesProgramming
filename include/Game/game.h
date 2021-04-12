// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _GAME_H_
#define _GAME_H_ 1

#include "ECS/ecs_manager.h"
#include "ECS/Components/transform_component.h"
#include "ECS/Components/render_component.h"
#include "ECS/Components/input_component.h"
#include "ECS/Components/rigid_body.h"
#include "Graphics/shader.h"
#include "Graphics/Materials/color_material.h"

#include "reactphysics3d/collision/RaycastInfo.h"

#include "Core/network_manager.h"
#include "Core/engine.h"
#include "Core/scripting_manager.h"

#include "Game/moving_cube.h"
#include "Game/door.h"

#include <vector>

namespace Aloe {

  class Game;

  /** Triggers event listener*/
  class MyEventListerner : public reactphysics3d::EventListener {
  public:   
    Game* game;

    void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData);
  };

  /** Raycast event listener*/
  class MyRaycastListener : public reactphysics3d::RaycastCallback {
  public:
    
    MovingCube* cube;
    virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& raycastInfo) override;
  };

  enum MaterialColor {

    Color_White,
    Color_Yellow,
    Color_Red,
    Color_Blue,
    Color_Brown,

    Color_MAX,
  };

  class Game {
  public:
    Game();
    ~Game();

    /** Current ecs*/
    ECSManager* ecs;

    /** Physics Event listener*/
    MyEventListerner event_listener;
    NetworkManager net_manager;
    Server server;

    ScriptingManager script_manager;

    /** Starts the game*/
    void Start();
    /** Updates the game*/
    void Update();
    /** Network update that will check events and respond according to the game state*/
    void NetworkUpdate();
    /** Destroys the current level*/
    void DestroyLevel();
    /** Updates all the elements of the game*/
    void GameplayElementsUpdate();

    /** Setups the server camera*/
    void CameraSetup();
    /** Setups the players, creating the entity and components*/
    void PlayersSetup();
    /** Player camera movement that receives the data from a client*/
    void PlayerCameraMovement(int client, float* front, float* input);

    /** Creates the dynamic data packet that contains the data that will be updated each frame to the client*/
    void CreateDynamicsDataPacket();
    /** Creates the update dynamic data packet that contains the data that will be updated each frame to the client*/
    void CreateUpdateDynamicsDataPacket();
    /** Creates the static map data that wont change each frame*/
    void CreateMapDataPacket();
    /** Updates the the update data packet to send it to the client*/
    void UpdateDynamicsDataPacket();
    /** Creates a moving cube and stores it internally*/
    u32 CreateMovingCube(glm::vec3 pos, MaterialColor col);
    /** Creates a door and stores it internally*/
    u32 CreateDoor(glm::vec3 pos, glm::vec3 scale);
    /** Creates a detector and stores it internally*/
    u32 CreateDetector(glm::vec3 pos, glm::vec3 scale, MaterialColor col);
    /** Creates a static and stores it internally*/
    Entity CreateStaticCube(glm::vec3 pos, glm::vec3 scale, MaterialColor col);

    /** Current level*/
    u32 level = 1;
    /** Map loading state for each player*/
    bool map_loaded[MAX_PLAYERS] = { false ,false};

    /**Players */
    Entity players[MAX_PLAYERS];

    /**Static map data*/
    std::vector<Entity>map_static;
    /**Static map TransformComponents to send to the clients*/
    std::vector<TransformComponent*>map_static_trs;

    /**Dynamic map data*/
    std::vector<Entity>map_dynamic;
    /**Dynamic map TransformComponents to send to the clients*/
    std::vector<TransformComponent*>map_dynamic_trs;

    /**Moving cubes*/
    std::vector<MovingCube> moving_cubes;

    /**Detectors*/
    std::vector<DetectorCube> detectors;

    /**Doors*/
    std::vector<Door> doors;

    /** Diffeent materials*/
    ColorMaterial* materials[Color_MAX];

    /** Cubes mesh*/
    Mesh *cube_mesh;

    /** Create map packet*/
    scoped_array<float> create_map_packet;
    /** Create map dynamics packet*/
    scoped_array<float> create_map_dynamics_packet;
    /** Update map dynamics packet*/
    scoped_array<float> update_map_dynamics_packet;
    
    /** Lua function for CreateStaticBlock*/
    static int lua_CreateStaticBlock(lua_State* L);
    /** Lua function for CreateDetectorBlock*/
    static int lua_CreateDetectorBlock(lua_State* L);
    /** Lua function for CreateMovingBlock*/
    static int lua_CreateMovingBlock(lua_State* L);
    /** Lua function for CreateDoorBlock*/
    static int lua_CreateDoorBlock(lua_State* L);
    /** Lua function for SetDoorMovingBlocks*/
    static int lua_SetDoorMovingBlocks(lua_State* L);
    /** Lua function for SetPlayerPositions*/
    static int lua_SetPlayerPositions(lua_State* L);


  };


}

#endif
