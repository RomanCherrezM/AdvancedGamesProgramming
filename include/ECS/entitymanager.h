#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_ 1

#include "Core/platform_types.h"
#include "ECS/ecs_constants.h"

#include <queue>
#include <array>

namespace Aloe {

  class EntityManager {

  public:
    EntityManager() {
      currentEntityCount = 0;

      //  Create the available entity IDs
      for (u32 i = 0; i < MAX_ENTITIES; ++i) {
        availableEntities.push(i);
      }
    }

    Entity CreateEntity() {
      //  ASSERT: numberEntities >= MAX_ENTITIES

      Entity new_entity = availableEntities.front();
      availableEntities.pop();
      currentEntityCount++;

      return new_entity;
    }

    void DestroyEntity(Entity entity) {
      //  No longer valid signature
      signatures[entity].reset();

      //  Entity slot available
      availableEntities.push(entity);

      currentEntityCount--;
    }

    //  Used by ECSManager to set entities signature when components
    //  are added
    void SetSignature(Entity entity, Signature signature) {
      signatures[entity] = signature;
    }

    //  Signature getter
    Signature GetSignature(Entity entity) {
      return signatures[entity];
    }

  private:

    //  Stores entity ID in order to create new ones, if an 
    //  Entity is destroyed it will be available again
    std::queue<Entity> availableEntities;

    //  Entities signature that defines which components they
    //  have in order to filter them to the systems
    std::array<Signature, MAX_ENTITIES> signatures;

    //  Limits of how many entities there are
    u32 currentEntityCount;

  };
}

#endif