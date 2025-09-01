
#include <stdarg.h>
#include <stdlib.h>

#include "ecs.h"
#include "mask.h"
#include "ecs_util.h"


int ecs_init_system(ECSWorld_t* _world, ECSSystem_f system, uint8_t typeSystem, ...)
{
    if (_world == NULL || system == NULL) return 0;

    va_list args;
    va_start(args, typeSystem);

    ECSSystems_t* systems = NULL;
    
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
        break;
    }

    if (systems == NULL)
    {
        va_end(args);
        return  0;
    }
    
    for (size_t i = 0; i < systems->size; i++)
    {
        if (systems->systems[i].system == system)
        {
            va_end(args);
            return 0;
        }
    }
    
    ECSMask_t mask = get_empty_mask();
    ECSComponentId comp;
    size_t index = 0;
    void* ptr = NULL;
    
    do
    {
        comp = va_arg(args, ECSComponentId);

        if (comp == 0) continue;

        mask_xor(&mask, comp - 1);
    } while (comp);
    
    va_start(args, typeSystem);
    
    ECSSystem_t sys = {system, mask, NULL, 1};

    if (mask.count > 0)
    {
        sys.compIds = calloc(mask.count, sizeof(*(sys.compIds)));

        if (sys.compIds != NULL)
        {
            do
            {
                comp = va_arg(args, ECSComponentId);

                if (comp == 0) continue;

                sys.compIds[index] = comp;
                index++;
            } while (comp && index < mask.count);
        }
    }
    
    va_end(args);

    if (systems->systems == NULL)
    {
        ptr = calloc(1, sizeof(*(systems->systems)));
    }
    else
    {
        ptr = realloc(systems->systems, (systems->size + 1) * sizeof(*(systems->systems)));
    }
    
    if (ptr == NULL)
    {
        if (sys.compIds != NULL) free(sys.compIds);
        mask_destroy(mask);
        return  0;
    }
    else
    {
        systems->systems = ptr;
    }

    systems->systems[systems->size] = sys;

    systems->size++;

    return 1;
}


void ecs_turn_system(ECSWorld_t* _world, ECSSystem_f system, uint8_t typeSystem, uint8_t status)
{
    ECSSystem_t* sys = util_get_system(_world, system, typeSystem);

    if (sys != NULL)
    {
        sys->online = status;
    }
}
