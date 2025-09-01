
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ecs.h"
#include "mask.h"

static ECSId _worldId = 0;

ECSWorld_t* init_world()
{
    ECSWorld_t* _world = calloc(1, sizeof(ECSWorld_t));

    if (_world == NULL) return NULL;

    _world->worldId = _worldId;
    _worldId++;
    return _world;
}

void destroyEntity(ECSEntity_t entity)
{
    for (size_t i = 0; i < entity.mask.size; i++)
    {
        if (entity.components[i] != NULL) free(entity.components[i]);
    }
    
    if (entity.components != NULL) free(entity.components);
    if (entity.entIds != NULL) free(entity.entIds);
    if (entity.position != NULL) free(entity.position);
    mask_destroy(entity.mask);
}

void destroyEntities(ECSEntities_t entities)
{
    for (size_t i = 0; i < entities.size; i++)
    {
        destroyEntity(entities.entities[i]);
    }
    
    if (entities.entities != NULL) free(entities.entities);
}



void destroyComponents(ECSComponents_t components)
{
    if (components.components != NULL) free(components.components);
}




void destroySystems(ECSSystems_t systems)
{
    for (size_t i = 0; i < systems.size; i++)
    {
        if (systems.systems[i].compIds != NULL) free(systems.systems[i].compIds);
        mask_destroy(systems.systems[i].mask);
    }
    
    if (systems.systems != NULL) free(systems.systems);
}

void destroyConfSystem(ECSConfigSystems_t conf)
{
    destroySystems(conf.initSystems);
    destroySystems(conf.runSystems);
    destroySystems(conf.destroySystems);
}

void destroy_world(ECSWorld_t* _world)
{
    if (_world == NULL) return  ;

    destroyConfSystem(_world->confSystem);
    destroyComponents(_world->components);
    destroyEntities(_world->entities);

    free(_world);
}