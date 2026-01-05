cmake_minimum_required(VERSION 3.16)
project(SFML_ImGui_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 🔹 SFML dynamique
set(SFML_STATIC_LIBRARIES OFF)
set(BUILD_SHARED_LIBS ON)

# 🔹 Chemin SFML (Ubuntu)
set(SFML_DIR "/usr/lib/x86_64-linux-gnu/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# 🔹 Chemin ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")
if(NOT EXISTS "${IMGUI_DIR}/imgui.h")
    message(FATAL_ERROR "Le dossier ImGui n'existe pas ou n'a pas imgui.h : ${IMGUI_DIR}")
endif()

# 🔹 ImGui-SFML
set(IMGUI_SFML_USE_STATIC_LIBS OFF)
add_subdirectory(external/imgui-sfml)

# 🔹 Executable
add_executable(${PROJECT_NAME} main.cpp)

# 🔹 Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${IMGUI_DIR}                  # pour imgui.h
    ${IMGUI_DIR}/misc/cpp         # pour imgui_stdlib.h
    ${SFML_INCLUDE_DIRS}
)

# 🔹 Link libraries
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML
)
