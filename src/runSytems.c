
#include <stdlib.h>

#include "ecs.h"
#include "ecs_util.h"
#include "mask.h"

ECS_RETURN ecs_run_system(ECSWorld_t* _world, uint8_t typeSystem, uint8_t emptySystem)
{
    if (_world == NULL) return ECS_RETURN_BREAK;

    ECSSystems_t* systems;
    ECSEntities_t* entities = &(_world->entities);

    switch (typeSystem)
    {
    case ECS_SYSTEM_INIT:
        systems = &(_world->confSystem.initSystems);
        break;
    case ECS_SYSTEM_RUN:
        systems = &(_world->confSystem.runSystems);
        break;
    case ECS_SYSTEM_DESTROY:
        systems = &(_world->confSystem.destroySystems);
        break;
    default:
        return ECS_RETURN_BREAK; 
        break;
    }

    

    ECSSystem_t sys;
    ECSIter_t iter;
    iter._world = _world;
    uint8_t res = 1;

    for (size_t i = 0; i < systems->size; i++)
    {
        sys = systems->systems[i];

        if (sys.online == 0) continue;
        
        iter.compIds = sys.compIds;

        if (sys.mask.count == 0 && emptySystem)
        {
            sys.system(&iter);
            continue;
        }

        for (size_t j = 0; j < entities->size; j++)
        {
            if (mask_inside(entities->entities[j].mask, sys.mask))
            {
                
                iter.entity = entities->entities + j;
                res = sys.system(&iter);

                if (res == ECS_RETURN_BREAK)
                    return res;
            }
        }
        
    }
    
    return ECS_RETURN_CONTINUE;
}