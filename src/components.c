
#include <stdlib.h>
#include <string.h>
#include "ecs.h"


ECSComponentId ecs_append_component(ECSWorld_t* _world, ECSComponent_t comp)
{
    if (_world == NULL) return  0;

    for (size_t i = 0; i < _world->components.size; i++)
    {
        if (strcmp(comp.nameComp, _world->components.components[i].nameComp) == 0) return (i + 1);
    }

    void* ptr = NULL;
    if (_world->components.components == NULL)
    {
        ptr = calloc(1, sizeof(*(_world->components.components)));
    }
    else
    {
        ptr = realloc(_world->components.components, (_world->components.size + 1) * sizeof(*(_world->components.components)));
    }

    if (ptr == NULL) return 0;

    _world->components.components = ptr;

    _world->components.components[_world->components.size] = comp;
    
    _world->components.size++;

    return _world->components.size;
}