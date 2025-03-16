cmake_minimum_required(VERSION 3.14)
project(RRT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
find_package(PkgConfig REQUIRED)

pkg_check_modules(YAMLCPP REQUIRED yaml-cpp>=0.5) 
# Add the include dirs
include_directories(include ${YAMLCPP_INCLUDE_DIRS})

#Set the source files
set(SOURCES
    src/rrt.cpp
    src/map.cpp
    main.cpp
)

# Create executable
add_executable(pathplan ${SOURCES})

# Link SFML libraries
target_link_libraries(pathplan 
    PRIVATE SFML::Graphics SFML::Window SFML::System
    ${YAMLCPP_LIBRARIES}
)
