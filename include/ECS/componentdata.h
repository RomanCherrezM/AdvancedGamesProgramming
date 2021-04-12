#ifndef _COMPONENTDATA_H_
#define _COMPONENTDATA_H_

#include "ECS/ecs_constants.h"

#include <unordered_map>
#include <array>
#include <assert.h>

namespace Aloe {

  class IComponentData {
  public:
    virtual ~IComponentData() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
  };

  template <typename T>
  class ComponentData : public IComponentData {

  public:

    ComponentData() {
      size = 0;
    }

    void InsertData(Entity entity, T component) {

      assert(mapEntitytoIndex.find(entity) == mapEntitytoIndex.end() && "Already existing component");

      componentData[size] = component;
      mapEntitytoIndex[entity] = size;
      mapIndextoEntity[size] = entity;

      size++;
    }

    void RemoveData(Entity entity) {

      assert(mapEntitytoIndex.find(entity) != mapEntitytoIndex.end() && "Non existing component");

      //  Put last element into the removed spot
      u32 indexOfRemoved = mapEntitytoIndex[entity];
      u32 indexLastElement = size - 1;
      componentData[indexOfRemoved] = componentData[indexLastElement];

      //  mapEntitytoIndex Given entity on last element now returns indexOfRemoved
      //  mapIndextoEntity Given indexOfRemoved now returns entity on last element 
      Entity lastEntity = mapIndextoEntity[indexLastElement];
      mapEntitytoIndex[lastEntity] = indexOfRemoved;
      mapIndextoEntity[indexOfRemoved] = lastEntity;

      //  Remove last element
      mapEntitytoIndex.erase(entity);
      mapIndextoEntity.erase(indexLastElement);

      size--;
    }

    T& GetData(Entity entity) {
      assert(mapEntitytoIndex.find(entity) != mapEntitytoIndex.end() && "Non existing component");

      return componentData[mapEntitytoIndex[entity]];
    }

    //  Only destroys the component if exists 
    void EntityDestroyed(Entity entity) override {
      if (mapEntitytoIndex.find(entity) != mapEntitytoIndex.end()) {
        RemoveData(entity);
      }
    }

  private:

    //  Contains all the components of type T for every
    //  entity, this means that each Entity can have
    //  only one component of type T
    std::array<T, MAX_ENTITIES> componentData;

    //  maps to get componentData index given an Entity or
    //  Entity given index
    std::unordered_map<Entity, u32> mapEntitytoIndex{};
    std::unordered_map<u32, Entity> mapIndextoEntity{};

    //  current size of the componentData array
    u32 size;
  };
}

#endif