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

# Add dependencies
find_package(SFML 2.6 COMPONENTS graphics network window audio system REQUIRED)
find_package(asio CONFIG REQUIRED)

# Set the compiler optimization flags
if (MSVC)
    # Windows Release flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")

    # Windows Debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /DDEBUG_MODE")
else()
    # GCC/Clang Release flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

    # GCC/Clang Debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG_MODE")
endif()

# Source all files
file(GLOB_RECURSE ECS_SOURCES CONFIGURE_DEPENDS "src/ecs/*.cpp")
file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS "src/client/*.cpp")
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS "src/server/*.cpp")

# Add the executables
add_executable(r-type_client ${CLIENT_SOURCES} ${ECS_SOURCES})
add_executable(r-type_server ${SERVER_SOURCES} ${ECS_SOURCES})

# Target configuration
foreach(target r-type_client r-type_server)
    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    )

    # Add compiler warnings
    if (MSVC)
        target_compile_options(${target} PRIVATE
            /W4            # applies gcc Wall/Wextra
            /permissive-   # enforce standard C++
            /Zc:preprocessor
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wshadow -Winit-self -Wredundant-decls
            -Wstrict-overflow=2 -Wundef -Wunreachable-code
            -Wwrite-strings
        )
    endif()

    # Add libraries
    target_link_libraries(${target} PRIVATE
        sfml-graphics
        sfml-network
        sfml-window
        sfml-audio
        sfml-system
        asio::asio
    )

    # Define compilation target
    target_compile_definitions(${target} PRIVATE
        PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}/"
    )
endforeach()

# Run installation
install(TARGETS r-type_client r-type_server
    RUNTIME DESTINATION bin
)

# Custom targets
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build . -- -j
    COMMENT "Building in debug mode with DEBUG_MODE defined"
)

add_custom_target(re
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} --build . --target all -- -j
)
