#ifndef _ECS_CONSTANTS_H_
#define _ECS_CONSTANTS_H_ 1

#include "Core/platform_types.h"

#include <bitset>

#define MAX_COMPONENTS 32
#define MAX_ENTITIES 1000

namespace Aloe {

  typedef u32 Entity;
  typedef std::bitset<MAX_COMPONENTS> Signature;
  typedef u8 ComponentType;

}

#endif