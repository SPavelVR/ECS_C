# ECS_C
 My implementation of the ECS concept in C
 
 example of working code:

    #include <stdio.h>
    #include <stdlib.h>
    
    #define _USE_MATH_DEFINES
    #include <math.h>
    
    #include <ecs.h>
    
    typedef struct 
    {
        float x, y;
    } Position, Vertex, Target;
    
    ECS_RETURN init_entities_sys(ECSIter_t* iter)
    {
        ECS_COMPONENT(iter->_world, Position);
        ECS_COMPONENT(iter->_world, Vertex);
        ECS_COMPONENT(iter->_world, Target);
    
        ECS_ENTITY(iter->_world,
                    ecs_value(Position, {17, 24}),
                    ecs_value(Vertex, {0, 0}),
                    ecs_value(Target, {92, 14})
                );
    
        ECS_ENTITY(iter->_world,
                    ecs_value(Position, {98, 1}),
                    ecs_value(Vertex, {0, 0}),
                    ecs_value(Target, {92, 105})
                );
    
        ECS_ENTITY(iter->_world,
                    ecs_value(Position, {87523, 11231}),
                    ecs_value(Vertex, {0, 0}),
                    ecs_value(Target, {1, 1})
                );
    
        ECS_ENTITY(iter->_world,
                    ecs_value(Position, {2, 2}),
                    ecs_value(Vertex, {0, 0}),
                    ecs_value(Target, {43, 44})
                );
    
        return ECS_RETURN_CONTINUE;
    }
    
    ECS_RETURN init_vertex_sys(ECSIter_t* iter)
    {
        Position* pos = ecs_field(iter, Position, 0);
        Vertex* vertex = ecs_field(iter, Vertex, 1);
        Target* targs = ecs_field(iter, Target, 2);
        
        float sum;
    
        for (size_t i = 0; i < iter->size; i++)
        {
            vertex[i].x = targs[i].x - pos[i].x;
            vertex[i].y = targs[i].y - pos[i].y;
    
            sum = sqrtf(vertex[i].x * vertex[i].x + vertex[i].y * vertex[i].y);
    
            vertex[i].x /= sum;
            vertex[i].y /= sum;
        }
        
        return ECS_RETURN_CONTINUE;
    }
    
    ECS_RETURN folder_target(ECSIter_t* iter)
    {
        Position* pos = ecs_field(iter, Position, 0);
        Vertex* vertex = ecs_field(iter, Vertex, 1);
    
        for (size_t i = 0; i < iter->size; i++)
        {
            pos[i].x += vertex[i].x;
            pos[i].y += vertex[i].y;
        }
    
        return ECS_RETURN_CONTINUE;
    }
    
    ECS_RETURN check_target(ECSIter_t* iter)
    {
        Position* pos = ecs_field(iter, Position, 0);
        Target* targ = ecs_field(iter, Target, 1);
    
        for (size_t i = 0; i < iter->size; i++)
        {
            if (targ[i].x - pos[i].x < 1 && targ[i].x - pos[i].x > -1 && targ[i].y - pos[i].y < 1 && targ[i].y - pos[i].y > -1)
            {
                return ECS_RETURN_BREAK;
            }
        }
    
        return ECS_RETURN_CONTINUE;
    }
    
    int main()
    {
        printf("Start ECS\n");
    
        ECSWorld_t* world = init_world();
    
        ECS_COMPONENT(world, Position);
        ECS_COMPONENT(world, Vertex);
        ECS_COMPONENT(world, Target);
    
        ECS_SYSTEM(world, init_entities_sys, ECS_SYSTEM_INIT, 0);
        ECS_SYSTEM(world, init_vertex_sys, ECS_SYSTEM_INIT, ecs_id(Position), ecs_id(Vertex), ecs_id(Target));
    
        ECS_SYSTEM(world, folder_target, ECS_SYSTEM_RUN, ecs_id(Position), ecs_id(Vertex));
        ECS_SYSTEM(world, check_target, ECS_SYSTEM_RUN, ecs_id(Position), ecs_id(Target));
    
        
        int res = ECS_INIT(world);
    
        while(ECS_RUN(world))
        {
            printf("One cicle run!\n");
        }
    
        ECS_DESTROY(world);
    
        destroy_world(world);
    
        printf("end of program!\n");
        return 0;
    }
