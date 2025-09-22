#Inicjalizacja
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(shooter)


#Include dirctories
include_directories(
    external/glad/include
    external/glfw-3.4/include
    external/stb_image/include
    external/glm/include
    external/assimp/include
    external/json/include
)

#myapp
# 1.  Grab every source file in src/ (recursively).
file(GLOB_RECURSE SRC
     CONFIGURE_DEPENDS          # auto-re-run CMake when new files appear
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(shooter ${SRC})

#Kopiowanie folderu res
add_custom_command(TARGET shooter PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/res
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/res
            ${CMAKE_BINARY_DIR}/res
    COMMENT "Copying resources!"
)

#glfw3
find_package(glfw3 3.4 REQUIRED)

#glad
add_library(glad STATIC external/glad/src/glad.c)

#GLM
find_package(glm CONFIG REQUIRED)

#assimp
find_package(assimp REQUIRED)

#nlhomann-json
find_package(nlohmann_json 3.12.0 REQUIRED)


target_link_libraries(shooter glfw glad glm::glm assimp::assimp nlohmann_json::nlohmann_json)


#GLFW expectcs these packages for MACOS
target_link_libraries(shooter
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
     "-framework CoreVideo"
)




