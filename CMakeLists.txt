cmake_minimum_required(VERSION 3.20)
project(r-type LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable multithreading for all builds
if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
    include(ProcessorCount)
    ProcessorCount(N)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

find_package(SFML 2.6 COMPONENTS graphics network window audio system REQUIRED)
find_package(asio CONFIG REQUIRED)

# ---- Compiler flags ----
if (MSVC)
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Release>:/O2 /DNDEBUG>
        $<$<CONFIG:Debug>:/Zi /DDEBUG_MODE>
    )
else()
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:Debug>:-g -DDEBUG_MODE>
    )
endif()


# ---- Source groups ----
file(GLOB_RECURSE ECS_SOURCES CONFIGURE_DEPENDS "src/ecs/*.cpp")
file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS "src/client/*.cpp")
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS "src/server/*.cpp")

# ---- Executables ----
add_executable(r-type_client ${CLIENT_SOURCES} ${ECS_SOURCES})
add_executable(r-type_server ${SERVER_SOURCES} ${ECS_SOURCES})

# ---- Include paths ----
foreach(target r-type_client r-type_server)
    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    )

    # Compiler warnings
    target_compile_options(${target} PRIVATE
        -Wall -Wextra -Winit-self -Wredundant-decls
        -Wshadow -Wstrict-overflow=2 -Wundef -Wunreachable-code
        -Wwrite-strings -pedantic --param max-inline-insns-single=200
    )

    target_link_libraries(${target} PRIVATE
        sfml-graphics
        sfml-network
        sfml-window
        sfml-audio
        sfml-system
        asio::asio
    )


    # Definitions
    target_compile_definitions(${target} PRIVATE
        PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}/"
    )
endforeach()

# ---- Installation ----
install(TARGETS r-type_client r-type_server
    RUNTIME DESTINATION bin
)

# ---- Custom targets ----
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build . -- -j
    COMMENT "Building in debug mode with DEBUG_MODE defined"
)

add_custom_target(re
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} --build . --target all -- -j
)