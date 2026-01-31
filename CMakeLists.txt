cmake_minimum_required(VERSION 3.15)
project(CrushFuzz VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} SHARED src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    ${LV2_INCLUDE_DIRS}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -O3
    -march=native
    -ffast-math
    -fPIC
    -fvisibility=hidden
)

if(NOT APPLE)
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--no-undefined")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "CrushFuzz"
)

if(NOT DEFINED LV2_INSTALL_DIR)
    set(LV2_INSTALL_DIR "$ENV{HOME}/.lv2/${PROJECT_NAME}.lv2")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${LV2_INSTALL_DIR})
install(FILES manifest.ttl crushfuzz.ttl DESTINATION ${LV2_INSTALL_DIR})

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Install destination: ${LV2_INSTALL_DIR}")
