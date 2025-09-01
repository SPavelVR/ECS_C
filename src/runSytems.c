
#include <stdlib.h>
#include <stdio.h>

#include "ecs.h"
#include "ecs_util.h"
#include "mask.h"

ECS_RETURN ecs_run_system(ECSWorld_t* _world, uint8_t typeSystem, uint8_t emptySystem)
{
    if (_world == NULL) return ECS_RETURN_BREAK;

    ECSSystems_t systems;
    ECSEntities_t entities = _world->entities;

    switch (typeSystem)
    {
    case ECS_SYSTEM_INIT:
        systems = _world->confSystem.initSystems;
        break;
    case ECS_SYSTEM_RUN:
        systems = _world->confSystem.runSystems;
        break;
    case ECS_SYSTEM_DESTROY:
        systems = _world->confSystem.destroySystems;
        break;
    default:
        return ECS_RETURN_BREAK; 
        break;
    }

    

    ECSSystem_t sys;
    ECSIter_t iter;

    for (size_t i = 0; i < systems.size; i++)
    {
        sys = systems.systems[i];

        if (sys.online == 0) continue;
        
        if (sys.mask.count == 0 && emptySystem)
        {
            sys.system(NULL);
            continue;
        }
        iter.compIds = sys.compIds;
        iter._world = _world;

        for (size_t j = 0; j < entities.size; j++)
        {
            if (mask_inside(entities.entities[j].mask, sys.mask))
            {
                iter.entity = entities.entities[j];
                iter.size = iter.entity.size;
                sys.system(&(iter));
            }
        }
        
    }
    
}