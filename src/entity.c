

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ecs.h"
#include "mask.h"
#include "ecs_util.h"

#define WORLD_ENT(_index)   \
    _world->entities.entities[_index]

int ecs_creat_entity(ECSWorld_t* _world, ECSMask_t mask, va_list args)
{
    if (_world->entities.entities == NULL)
    {
        _world->entities.entities = calloc(1, sizeof(ECSEntity_t));

        if (_world->entities.entities == NULL) return 0;
    }
    else
    {
        _world->entities.entities = realloc(_world->entities.entities, (_world->entities.size + 1) * sizeof(ECSEntity_t));

        if (_world->entities.entities == NULL) return 0;
    }

    size_t index = _world->entities.size;

    WORLD_ENT(index).mask = mask;
    WORLD_ENT(index).components = calloc(mask.count, sizeof(void*));
    WORLD_ENT(index).position = calloc(mask.count, sizeof(ECSComponentId));
    WORLD_ENT(index).entIds = calloc(8, sizeof(ECSEntityId));
    WORLD_ENT(index).size = 0;
    WORLD_ENT(index).capacity = 8;

    size_t pass = 0;
    ECSValue_t value;

    do
    {
        value = va_arg(args, ECSValue_t);

        if (value.compId == 0) continue;

        WORLD_ENT(index).position[pass] = value.compId;
        WORLD_ENT(index).components[pass] = calloc(WORLD_ENT(index).capacity, value.sizeComp);
        pass++;
    } while (value.compId);

    _world->entities.size++;
    
    return 1;
}

int ecs_append_entity(ECSWorld_t* _world, ECSEntityId id, size_t index, va_list args)
{
    if (WORLD_ENT(index).components == NULL || WORLD_ENT(index).entIds == NULL || WORLD_ENT(index).position == NULL) return 0;

    int res = 0;

    if (WORLD_ENT(index).size == WORLD_ENT(index).capacity)
    {
        WORLD_ENT(index).capacity += 8;
        res = 1;
        WORLD_ENT(index).entIds = realloc(WORLD_ENT(index).entIds, (WORLD_ENT(index).capacity) * sizeof(ECSEntityId));
    }

    ECSValue_t value;
    size_t pos;

    do
    {
        value = va_arg(args, ECSValue_t);

        if (value.compId == 0) continue;

        pos = util_get_index(WORLD_ENT(index).position, WORLD_ENT(index).mask.count, value.compId);

        if (res)
        {
            WORLD_ENT(index).components[pos] = realloc(WORLD_ENT(index).components[pos], WORLD_ENT(index).capacity * value.sizeComp);
        }

        memcpy( (char*) WORLD_ENT(index).components[pos] + WORLD_ENT(index).size * value.sizeComp, value.comp, value.sizeComp);
    } while (value.compId);

    if (id == 0)
    {
        id = _world->entities.maxId + 1;
        _world->entities.maxId++;
    }

    WORLD_ENT(index).entIds[WORLD_ENT(index).size] = id;

    WORLD_ENT(index).size++;

    return 1;
}

ECSEntityId ecs_init_entity(ECSWorld_t* _world, ...)
{

    if (_world == NULL) return 0;

    va_list factor;
    va_start(factor, _world);

    ECSMask_t mask = get_empty_mask();
    ECSValue_t value;

    do
    {
        value = va_arg(factor, ECSValue_t);

        if (value.compId)
        {
            mask_xor(&mask, value.compId - 1);
        }
    } while (value.compId);

    if (mask.count == 0)
    {
        va_end(factor);
        mask_destroy(mask);
        return 0;
    }

    va_start(factor, _world);
    

    for (size_t i = 0; i < _world->entities.size; i++)
    {
        if (mask_equal(_world->entities.entities[i].mask, mask))
        {
            mask_destroy(mask);
            
            if (ecs_append_entity(_world, 0, i, factor))
            {
                va_end(factor);
                return _world->entities.maxId;
            }

            va_end(factor);
            return 0;
        }
    }


    if (ecs_creat_entity(_world, mask, factor) == 0)
    {
        mask_destroy(mask);
        va_end(factor);
        return 0;
    }

    if (ecs_append_entity(_world, 0, _world->entities.size - 1, factor) == 0)
    {
        mask_destroy(mask);
        va_end(factor);
        return 0;
    }

    return _world->entities.maxId;
}


int ecs_destroy_entity_up(ECSWorld_t* _world, ECSEntityId entId, ECSEntity_t* entity)
{
    if (_world == NULL || entId == 0) return 0;

    if (entity == NULL) return ecs_destroy_entity(_world, entId);

    size_t size = util_get_index((ECSComponentId*) entity->entIds, entity->size, (ECSComponentId) entId);

    if (size == entity->size) return ecs_destroy_entity(_world, entId);

    else if (size + 1 == entity->size)
    {
        entity->size--;
        return 1;
    }

    ECSComponent_t* comp = NULL;
    
    for (size_t i = 0; i < entity->mask.count; i++)
    {
        comp = util_get_component_for_id(_world, entity->position[i]);

        if (comp == NULL) continue;

        memcpy((char*) entity->components[i] + size * comp->sizeComp, (char*) entity->components[i] + (entity->size - 1) * comp->sizeComp, comp->sizeComp);
    }

    entity->entIds[size] = entity->entIds[entity->size - 1];

    entity->size--;
    return 1;

}

int ecs_destroy_entity(ECSWorld_t* _world, ECSEntityId entId)
{
    if (_world == NULL || entId == 0) return 0;

    ECSEntity_t* entity = util_get_entity_for_id(_world, entId);
    ECSComponent_t* comp = NULL;
    
    if (entity == NULL) return 0;

    size_t size = util_get_index((ECSComponentId*) entity->entIds, entity->size, (ECSComponentId) entId);

    if (size + 1 == entity->size)
    {
        entity->size--;
        return 1;
    }

    for (size_t i = 0; i < entity->mask.count; i++)
    {
        comp = util_get_component_for_id(_world, entity->position[i]);

        if (comp == NULL) continue;

        memcpy((char*) entity->components[i] + size * comp->sizeComp, (char*) entity->components[i] + (entity->size - 1) * comp->sizeComp, comp->sizeComp);
    }

    entity->entIds[size] = entity->entIds[entity->size - 1];

    entity->size--;
    return 1;
}

void* ecs_get_component_entity(ECSWorld_t* _world, ECSEntityId _entId, ECSComponentId compId, size_t sizeComp)
{
    if (_world == NULL || _entId == 0 || compId == 0 || _entId > _world->entities.maxId) return NULL;

    size_t pos = 0;
    void* ptr = NULL;

    for (size_t i = 0; i < _world->entities.size; i++)
    {
        if (mask_get(WORLD_ENT(i).mask, compId - 1))
        {
            for (size_t j = 0; j < WORLD_ENT(i).size; j++)
            {
                if (WORLD_ENT(i).entIds[j] == _entId)
                {
                    pos = util_get_index(WORLD_ENT(i).position, WORLD_ENT(i).mask.count, compId);

                    if (pos == WORLD_ENT(i).mask.count) return NULL;

                    ptr = WORLD_ENT(i).components[pos];

                    return ((char*) ptr + j * sizeComp);
                }
            }
        }
    }
}


ECSEntityId ecs_inclusion_entity(ECSWorld_t* _world, ECSEntityId _entId, ...)
{
    if (_world == NULL || _entId == 0 || _entId > _world->entities.maxId) return 0;
    
    ECSEntity_t* entity = util_get_entity_for_id(_world, _entId);

    if (entity != NULL) return 0;

    va_list factor;
    va_start(factor, _entId);

    ECSMask_t mask = get_empty_mask();
    ECSValue_t value;

    do
    {
        value = va_arg(factor, ECSValue_t);

        if (value.compId)
        {
            mask_xor(&mask, value.compId - 1);
        }
    } while (value.compId);

    if (mask.count == 0)
    {
        va_end(factor);
        mask_destroy(mask);
        return 0;
    }

    va_start(factor, _entId);
    

    for (size_t i = 0; i < _world->entities.size; i++)
    {
        if (mask_equal(_world->entities.entities[i].mask, mask))
        {
            mask_destroy(mask);

            if (ecs_append_entity(_world, _entId, i, factor))
            {
                va_end(factor);
                return _entId;
            }

            va_end(factor);
            return 0;
        }
    }


    if (ecs_creat_entity(_world, mask, factor) == 0)
    {
        mask_destroy(mask);
        va_end(factor);
        return 0;
    }

    if (ecs_append_entity(_world, _entId, _world->entities.size - 1, factor) == 0)
    {
        mask_destroy(mask);
        va_end(factor);
        return 0;
    }

    return _entId;
}