
#include <stddef.h>
#include "ecs_util.h"


size_t util_get_index(ECSComponentId* position, size_t size, ECSComponentId id)
{
    if (position == NULL) return size;

    for (size_t i = 0; i < size; i++)
    {
        if (position[i] == id) return i;
    }
    
    return size;
}

ECSEntity_t* util_get_entity_for_id(ECSWorld_t* world, ECSEntityId entId)
{
    if (world == NULL || entId == 0 || entId > world->entities.maxId) return NULL;

    for (size_t i = 0; i < world->entities.size; i++)
    {
        for (size_t j = 0; j < world->entities.entities[i].size; j++)
        {
            if (world->entities.entities[i].entIds[j] == entId) return &(world->entities.entities[i]);
        }
    }

    return NULL;
}
ECSComponent_t* util_get_component_for_id(ECSWorld_t* world, ECSComponentId compId)
{
    if (world == NULL || compId == 0 || (compId - 1) >= world->components.size) return NULL;

    return &(world->components.components[compId - 1]);
    
}

ECSSystem_t* util_get_system(ECSWorld_t* world, ECSSystem_f system, uint8_t typeSystem)
{
    if (world == NULL) return NULL;

    ECSSystems_t* sys = (ECSSystems_t*) &(world->confSystem);

    for (size_t i = 0; i < sys[typeSystem].size; i++)
    {
        if (sys[typeSystem].systems[i].system == system)
        {
            return (sys[typeSystem].systems + i);
        }
    }
    
    return NULL;
}