
#include <stddef.h>

#include "ecs.h"
#include "ecs_util.h"
#include "mask.h"


void* ecs_get_for_iter(ECSIter_t* _iter, ECSDesp _desp)
{
    if (_iter == NULL || _iter->compIds == NULL) return NULL;

    ECSComponentId comp = _iter->compIds[_desp];
    comp = util_get_index(_iter->entity.position, _iter->entity.mask.count, comp);

    return _iter->entity.components[comp];
}