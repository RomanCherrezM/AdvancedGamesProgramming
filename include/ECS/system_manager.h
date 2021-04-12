#ifndef _SYSTEM_MANAGER_H_
#define _SYSTEM_MANAGER_H_ 1

#include "ECS/ecs_constants.h"
#include "ECS/Systems/system.h"

#include <memory>
#include <unordered_map>
#include <assert.h>

namespace Aloe {

	class SystemManager {
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem() {
			const char* typeName = typeid(T).name();

			assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature) {
			const char* typeName = typeid(T).name();

			assert(systems.find(typeName) != systems.end() && "System used before registered.");
			signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity) {
			// Erase a destroyed entity from all system lists
			// mEntities is a set so no check needed
			for (auto const& pair : systems) {
				auto const& system = pair.second;

				system->entities.erase(entity);
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature) {
			// Notify each system that an entity's signature changed
			for (auto const& pair : systems) {
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = signatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature) {
					system->entities.insert(entity);
				}
				// Entity signature does not match system signature - erase from set
				else {
					system->entities.erase(entity);
				}
			}
		}

	private:
		std::unordered_map<const char*, Signature> signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};

}
#endif
