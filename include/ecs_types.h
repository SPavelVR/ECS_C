

#ifndef __ECS_TYPES_H
#define __ECS_TYPES_H

#include <stdint.h>

/*      __ECS_ID_TYPE - базовый тип для всех Id в ECS */
#ifndef __ECS_ID_TYPE
#define __ECS_ID_TYPE uint32_t
#endif


typedef __ECS_ID_TYPE ECSId;

typedef ECSId ECSComponentId;
typedef ECSId ECSEntityId;
typedef ECSId ECSSystemId;

typedef ECSId ECSDesp;

typedef int8_t ECS_RETURN;

#define ECS_RETURN_BREAK 0
#define ECS_RETURN_CONTINUE 1

#define ECS_SYSTEM_INIT 0
#define ECS_SYSTEM_RUN 1
#define ECS_SYSTEM_DESTROY 2

typedef struct ECSMask_t ECSMask_t;

typedef struct ECSValue_t ECSValue_t;

typedef struct ECSEntity_t ECSEntity_t;
typedef struct ECSEntities_t ECSEntities_t;

typedef struct ECSComponent_t ECSComponent_t;
typedef struct ECSComponents_t ECSComponents_t;

typedef struct ECSIter_t ECSIter_t;
typedef struct ECSSystem_t ECSSystem_t;
typedef struct ECSSystems_t ECSSystems_t;
typedef struct ECSConfigSystems_t ECSConfigSystems_t ;

typedef struct ECSWorld_t ECSWorld_t;

typedef ECS_RETURN (*ECSSystem_f) (ECSIter_t* iter);

struct ECSMask_t
{
    uint64_t* mask;
    size_t size;
    size_t count;
};

struct ECSValue_t
{
    ECSComponentId compId;
    size_t sizeComp;
    void* comp;
};

struct ECSEntity_t
{
    ECSMask_t mask;
    ECSComponentId* position;
    void** components;
    ECSEntityId* entIds;
    size_t size;
    size_t capacity;
};

struct ECSEntities_t
{
    ECSEntity_t* entities;
    size_t size;
    ECSEntityId maxId;
};

struct ECSComponent_t
{
    const char* nameComp;
    size_t sizeComp;
};

struct ECSComponents_t
{
    ECSComponent_t* components;
    size_t size;
};

struct ECSIter_t
{
    ECSComponentId* compIds;
    ECSEntity_t* entity;
    ECSWorld_t* _world;
};

struct ECSSystem_t
{
    ECSSystem_f system;
    ECSMask_t mask;
    ECSComponentId* compIds;
    uint8_t online;
};

struct ECSSystems_t
{
    ECSSystem_t* systems;
    size_t size;
};

struct ECSConfigSystems_t
{
    ECSSystems_t initSystems;
    ECSSystems_t runSystems;
    ECSSystems_t destroySystems;
};

struct ECSWorld_t
{
    ECSId worldId;

    ECSConfigSystems_t confSystem;
    ECSComponents_t components;
    ECSEntities_t entities;
};

#endif