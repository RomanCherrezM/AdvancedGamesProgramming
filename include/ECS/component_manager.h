#ifndef _COMPONENT_MANAGER_H_
#define _COMPONENT_MANAGER_H_ 1

#include "ECS/componentdata.h"

#include <unordered_map>
#include <memory>
#include <assert.h>

namespace Aloe {

	class ComponentManager {

	public:
		template<typename T>
		void RegisterComponent() {
			const char* typeName = typeid(T).name();

			assert(componentTypesMap.find(typeName) == componentTypesMap.end() && "Registering component type more than once.");
			componentTypesMap.insert({ typeName, nextComponentType });
			componentDataMap.insert({ typeName, std::make_shared<ComponentData<T>>() });
			++nextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType() {
			const char* typeName = typeid(T).name();

			assert(componentTypesMap.find(typeName) != componentTypesMap.end() && "Component not registered before use.");

			return componentTypesMap[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, T component) {
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity) {
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity) {
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity) {
			for (auto const& pair : componentDataMap) {
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<const char*, ComponentType> componentTypesMap{};
		std::unordered_map<const char*, std::shared_ptr<IComponentData>> componentDataMap{};

		ComponentType nextComponentType;

		template<typename T>
		std::shared_ptr<ComponentData<T>> GetComponentArray() {
			const char* typeName = typeid(T).name();
			//assert component not registered
			return std::static_pointer_cast<ComponentData<T>>(componentDataMap[typeName]);
		}
	};

}

#endif