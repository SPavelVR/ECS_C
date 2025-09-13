

#ifndef __ECS_CONFIG_H
#define __ECS_CONFIG_H

#include "ecs.h"

ECSComponentId ecs_append_component(ECSWorld_t* _world, ECSComponent_t comp);

ECSEntityId ecs_init_entity(ECSWorld_t* _world, ...);
int ecs_destroy_entity(ECSWorld_t* _world, ECSEntityId entId);
void* ecs_get_component_entity(ECSWorld_t* _world, ECSEntityId _entId, ECSComponentId compId, size_t sizeComp);
ECSEntityId ecs_inclusion_entity(ECSWorld_t* _world, ECSEntityId _entId, ...);
int ecs_destroy_entity_with_ep(ECSWorld_t* _world, ECSEntityId entId, ECSEntity_t* entity, size_t pos);

int ecs_init_system(ECSWorld_t* _world, ECSSystem_f system, uint8_t typeSystem, ...);

void* ecs_get_for_iter(ECSIter_t* _iter, ECSDesp _desp);

ECSWorld_t* init_world();
void destroy_world(ECSWorld_t* world);

ECS_RETURN ecs_run_system(ECSWorld_t* _world, uint8_t typeSystem, uint8_t emptySystem);


void ecs_turn_system(ECSWorld_t* _world, ECSSystem_f system, uint8_t typeSystem, uint8_t status);

#endif