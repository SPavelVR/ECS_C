

#ifndef __ECS_MASK_H
#define __ECS_MASK_H

#include "ecs.h"

ECSMask_t get_empty_mask();

ECSId mask_xor(ECSMask_t* mask, ECSId desp);

ECSId mask_get(ECSMask_t mask, ECSId desp);

void mask_destroy(ECSMask_t mask);

int mask_equal(ECSMask_t mask1, ECSMask_t mask2);

int mask_inside(ECSMask_t mask, ECSMask_t ins);

#endif