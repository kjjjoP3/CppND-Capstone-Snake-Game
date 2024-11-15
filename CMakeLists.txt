cmake_minimum_required(VERSION 3.10)

project(SnakeGame)

set(SOURCE_FILES 
    src/main.cpp 
    src/controller.cpp 
    src/game.cpp 
    src/renderer.cpp 
    src/snake.cpp 
    src/scoremanager.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(SnakeGame ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(Threads REQUIRED)

# Clean up SDL2_LIBRARIES from any unwanted whitespace
string(REPLACE " " ";" SDL2_LIBRARIES_CLEANED ${SDL2_LIBRARIES})  # Replace spaces with semicolons
foreach(LIB ${SDL2_LIBRARIES_CLEANED})
    string(STRIP "${LIB}" LIB_CLEANED)  # Strip any leading/trailing whitespace
    target_link_libraries(SnakeGame ${LIB_CLEANED})
endforeach()

# Link pthread explicitly
target_link_libraries(SnakeGame Threads::Threads pthread)