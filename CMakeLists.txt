cmake_minimum_required(VERSION 4.0.0 FATAL_ERROR)

project(ECS_C VERSION 0.2 LANGUAGES C)


set(FILES   src/components.c
            src/world.c
            src/mask.c
            src/ecs_util.c
            src/entity.c
            src/system.c
            src/field.c
            src/runSytems.c
            )

# Создаем библиотеку
add_library(${PROJECT_NAME} STATIC ${FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Указываем где искать заголовочные файлы
target_include_directories(${PROJECT_NAME} PUBLIC include)