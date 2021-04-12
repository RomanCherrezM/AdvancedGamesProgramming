#ifndef _SYSTEM_H_
#define _SYSTEM_H_ 1

#include "ECS/ecs_constants.h"
#include <set>

namespace Aloe {

	class System {
	public:
		std::set<Entity> entities;
	};

}

#endif