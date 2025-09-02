
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ECS_H
#define __ECS_H

#include "ecs_types.h"
#include "ecs_config.h"


#define ecs_id(T)   CAVE_ECS_ID_##T

#define ecs_value(T, ...) \
    (ECSValue_t) {ecs_id(T), sizeof(T), &(T) __VA_ARGS__}


#define ECS_FINDSET_COMPONENT(_world, T)    \
    {   \
        ECSComponent_t comp = { \
            #T,         \
            sizeof(T)   \
        };  \
        ecs_id(T) = ecs_append_component(_world, comp); \
    }

#define ECS_COMPONENT(_world, T)    \
    ECSComponentId ecs_id(T) = 0;   \
    ECS_FINDSET_COMPONENT(_world, T)\
    (void) ecs_id(T);


#define ECS_ENTITY(_world, ...) \
    ecs_init_entity(_world, __VA_ARGS__, (ECSValue_t) {0, 0, NULL})

#define ECS_DESTROY_ENTITY(_world, _entity_id)  \
    ecs_destroy_entity(_world, _entity_id)

#define ECS_GET_COMPONENT_ENTITY(_world, _entId, T) \
    (T*) ecs_get_component_entity(_world, _entId, ecs_id(T), sizeof(T))

#define ECS_INCLUSION_ENTITY(_world, _entId, ...)   \
    ecs_inclusion_entity(_worldId, _entId, __VA_ARGS__, (ECSValue_t) {0, 0, NULL})

#define ecs_field(__iter, T, _disp)   \
    (T*) ecs_get_for_iter(__iter, _disp)


#define ECS_SYSTEM(__world, __system, __type_system, ...)   \
    ecs_init_system(__world, __system, __type_system, __VA_ARGS__, 0)

#define ECS_TURN_SYSTEM_ON(_world, _system_, _type_system)   \
    ecs_turn_system(_world, _system_, _type_system, 1)

#define ECS_TURN_SYSTEM_OFF(_world, _system_, _type_system)  \
    ecs_turn_system(_world, _system_, _type_system, 0)



#define ECS_INIT(_world)    \
    ecs_run_system(_world, ECS_SYSTEM_INIT, 1)

#define ECS_RUN(_world) \
    ecs_run_system(_world, ECS_SYSTEM_RUN, 0)

#define ECS_DESTROY(_world) \
    ecs_run_system(_world, ECS_SYSTEM_DESTROY, 1)




#endif

#ifdef __cplusplus
}
#endif
