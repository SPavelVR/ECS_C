

#ifndef __ECS_UTIL_H
#define __ECS_UTIL_H

#include "ecs.h"

size_t util_get_index(ECSId* position, size_t size, ECSId id);


ECSEntity_t* util_get_entity_for_id(ECSWorld_t* world, ECSEntityId entId);
ECSComponent_t* util_get_component_for_id(ECSWorld_t* world, ECSComponentId compId);
ECSSystem_t* util_get_system(ECSWorld_t* world, ECSSystem_f system, uint8_t typeSystem);

#endif