cmake_minimum_required(VERSION 3.0)

project(HaniwaSlayer_Proj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(SDL)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)

add_executable(HaniwaSlayer
    src/glad.c
    src/stb_image.c
    src/entity.cpp
    src/main.cpp)
target_include_directories(HaniwaSlayer PUBLIC SDL/include json/include)
target_link_libraries(HaniwaSlayer SDL2 nlohmann_json::nlohmann_json)
