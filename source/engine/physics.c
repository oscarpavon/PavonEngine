#include "physics.h"

#include <bullet/SharedMemory/PhysicsClientC_API.h>
#include <bullet/SharedMemory/PhysicsDirectC_API.h>

void pe_phy_init() {

  b3PhysicsClientHandle sm = 0;
  sm = b3ConnectPhysicsDirect();
}
