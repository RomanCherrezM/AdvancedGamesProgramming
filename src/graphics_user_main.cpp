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

#include "Graphics/shader.h"
#include "Graphics/Materials/basic_material.h"
#include "Graphics/Materials/water_material.h"
#include "Graphics/Materials/terrain_material.h"
#include "Graphics/Materials/grass_material.h"
#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include "Graphics/skybox.h"

#include "px_mem.h"

#include "gtc/matrix_transform.hpp"
#include <chrono>

#define CAMERA_SPEED 0.5f

namespace Aloe {

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

  void MoveCameraRight() {
  
    Engine* e = Engine::get();
    CameraComponent& c = e->ecs->GetComponent<CameraComponent>(e->main_camera);  
    c.position += glm::normalize(glm::cross(c.front, c.up))* CAMERA_SPEED;
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

  TerrainMaterial* terrain_mat;
  Mesh* terrain_mesh;
  void TerrainSetup() {
    ECSManager* ecs = GetGlobalECS();

    Entity plane_entity;
    plane_entity = ecs->CreateEntity();

    TransformComponent tr;
    tr.position = glm::vec3(0.0f, -60.0f, 0.0f);
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);


    tr.scale = glm::vec3(200.0f, 100.0f, 200.0f);

    Shader vertex_shader;
    vertex_shader.create(std::string("../../src/Shaders/shader_terrain.vert"), ShaderVertex);
    Shader fragment_shader;
    fragment_shader.create(std::string("../../src/Shaders/shader_terrain.frag"), ShaderFragment);

    terrain_mat = new TerrainMaterial;

    terrain_mat->create(vertex_shader, fragment_shader);
    terrain_mat->setupUniforms();

    terrain_mat->base_texture.create().loadTexture("../../resources/textures/terrain_base.jpg");
    terrain_mat->grass_texture.create().loadTexture("../../resources/textures/terrain_grass.jpg");
    terrain_mat->sand_texture.create().loadTexture("../../resources/textures/terrain_sand.jpg");

    RenderComponent re;
    terrain_mesh = CreateTerrainMesh(300, 300);
    re.mesh = terrain_mesh;
    re.material = terrain_mat;

    ecs->AddComponent<TransformComponent>(plane_entity, tr);
    ecs->AddComponent<RenderComponent>(plane_entity, re);
  }

  GrassMaterial* grass_mat;
  void GrassSetup() {
    ECSManager* ecs = GetGlobalECS();

    Entity plane_entity;
    plane_entity = ecs->CreateEntity();

    TransformComponent tr;
    tr.position = glm::vec3(0.0f, -60.0f, 0.0f);
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = glm::vec3(200.0f, 100.0f, 200.0f);

    Shader vertex_shader;
    vertex_shader.create(std::string("../../src/Shaders/shader_grass.vert"), ShaderVertex);
    Shader fragment_shader;
    fragment_shader.create(std::string("../../src/Shaders/shader_grass.frag"), ShaderFragment);
    Shader geometry_shader;
    geometry_shader.create(std::string("../../src/Shaders/shader_grass.geom"), ShaderGeometry);

    grass_mat = new GrassMaterial;

    grass_mat->create(vertex_shader, fragment_shader,geometry_shader);
    grass_mat->setDrawMode(Points);
    grass_mat->setupUniforms();

    RenderComponent re;
    re.mesh = CreateGrassMesh(terrain_mesh,0.6f);
    re.material = grass_mat;

    ecs->AddComponent<TransformComponent>(plane_entity, tr);
    ecs->AddComponent<RenderComponent>(plane_entity, re);
  }

  WaterMaterial* water_mat;
  void WaterSetup() {
    ECSManager* ecs = GetGlobalECS();

    Entity plane_entity;
    plane_entity = ecs->CreateEntity();

    TransformComponent tr;
    tr.position = glm::vec3(0.0f, -3.0f, 0.0f);
    tr.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    tr.scale = glm::vec3(200.0f, 40.0f, 200.0f);

    Shader vertex_shader;
    vertex_shader.create(std::string("../../src/Shaders/shader_water.vert"), ShaderVertex);
    Shader fragment_shader;
    fragment_shader.create(std::string("../../src/Shaders/shader_water.frag"), ShaderFragment);

    water_mat = new WaterMaterial;

    water_mat->create(vertex_shader, fragment_shader);
    water_mat->setupUniforms();
    water_mat->height_map1.create().loadTexture("../../resources/textures/water_height.jpg");
    water_mat->normal_map1.create().loadTexture("../../resources/textures/water_normal.jpg");
    water_mat->height_map2.create().loadTexture("../../resources/textures/water_height_2.jpg");
    water_mat->normal_map2.create().loadTexture("../../resources/textures/water_normal_2.jpg");
    water_mat->cubemap = skybox.getTexture();

    RenderComponent re;
    re.mesh = CreatePlaneMesh(1000,1000);
    re.material = water_mat;

    ecs->AddComponent<TransformComponent>(plane_entity, tr);
    ecs->AddComponent<RenderComponent>(plane_entity, re);

  }

  void RecompileShaders() {
  
    water_mat->recompileShaders();
    terrain_mat->recompileShaders();
    grass_mat->recompileShaders();

  }

  void ChangeWireframeState() {

    water_mat->setWireframe(true);
    terrain_mat->setWireframe(true);
    grass_mat->setWireframe(true);
  }

  void Aloe::Start() {

    CreateWindowContext(900,600,"Graphics Programming");

    SetupSkybox();
    TerrainSetup();
    WaterSetup();
    GrassSetup();

    Engine* e = Engine::get();
    ECSManager* ecs = GetGlobalECS();

    Entity entity;
    entity = ecs->CreateEntity();

    //Input Component
    InputComponent inputComp;

    AddInputBinding(inputComp, 'W',MoveCameraUp);
    AddInputBinding(inputComp, 'A',MoveCameraLeft);
    AddInputBinding(inputComp, 'S',MoveCameraDown);
    AddInputBinding(inputComp, 'D',MoveCameraRight);
    AddInputBinding(inputComp, 'C',RecompileShaders);
    AddInputBinding(inputComp, 'I',ChangeWireframeState);
    AddMouseInputBinding(inputComp, MoveCameraAround);

    ecs->AddComponent(entity, inputComp);

    // Camera
    camera_entity = ecs->CreateEntity();

    CameraComponent c;

    c.setViewMatrix(
      glm::vec3(0.f, 0.f, 10.f),
      glm::vec3(0.f, 0.f, -1.f),
      glm::vec3(0.f, 1.f, 0.f)).
      setProjectionMatrix(90.f, 0.1f, 1000.0f, 900, 600);

    ecs->AddComponent(camera_entity, c);
    SetMainCamera(camera_entity);

  }

  void Aloe::Update() {
    water_mat->setWireframe(false);
    terrain_mat->setWireframe(false);
    grass_mat->setWireframe(false);
  }

  void Aloe::Close() {

  }
}