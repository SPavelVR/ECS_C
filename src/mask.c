#include <stdlib.h>
#include "mask.h"

ECSMask_t get_empty_mask()
{
    ECSMask_t mask = {
        NULL,
        0,
        0
    };

    return mask;
}

ECSId mask_xor(ECSMask_t* mask, ECSId desp)
{
    ECSId strick = desp / (sizeof(*(mask->mask)) * 8);
    ECSId row = desp % (sizeof(*(mask->mask)) * 8);

    void* ptrM = NULL;
    size_t size = mask->size;

    if (mask->size == 0)
    {
        ptrM = calloc( strick + 1, sizeof(*(mask->mask)));

        if (ptrM == NULL) return 0;

        size = strick + 1;
    }
    else if (strick > mask->size)
    {
        ptrM = realloc(mask->mask, (strick + 1) * sizeof(*(mask->mask)));

        if (ptrM == NULL) return 0;

        size = strick + 1;
    }

    if (ptrM != NULL)
    {
        mask->mask = ptrM;
    }

    ECSId res = (mask->mask[strick]) & (1 << row);

    if (res)
    {
        mask->count--;
    }
    else
    {
        mask->count++;
        
    }

    mask->size = size;

    mask->mask[strick] = (mask->mask[strick]) ^ (1 << row);
    
    return res > 0;
}

ECSId mask_get(ECSMask_t mask, ECSId desp)
{
    ECSId strick = desp / (sizeof(*(mask.mask)) * 8);
    ECSId row = desp % (sizeof(*(mask.mask)) * 8);

    if (strick > mask.size) return 2;

    return ((mask.mask[strick]) & (1 << row)) > 0;
}

void mask_destroy(ECSMask_t mask)
{
    if (mask.mask != NULL) free(mask.mask);
}

int mask_equal(ECSMask_t mask1, ECSMask_t mask2)
{
    if (mask1.size != mask2.size || mask1.count != mask2.count) return  0;

    for (size_t i = 0; i < mask1.size; i++)
    {
        if (mask1.mask[i] != mask2.mask[i]) return 0;
    }
    
    return 1;
}

int mask_inside(ECSMask_t mask, ECSMask_t ins)
{
    if (mask.count < ins.count || mask.size < ins.size) return 0;

    for (size_t i = 0; i < ins.size; i++)
    {
        if ((mask.mask[i] & ins.mask[i]) != ins.mask[i]) return 0;
    }

    return 1;
}