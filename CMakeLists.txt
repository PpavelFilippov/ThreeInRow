cmake_minimum_required(VERSION 3.25)
project(3in1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_STANDARD 17)

add_executable(3in1 main.cpp
                    Header.h
                    UI.cpp
                    Object.cpp
                    Render.cpp
                    Builder.cpp
                    Bonuses.cpp
        )

set(SFML_DIR "C:\\SFML\\lib\\cmake\\SFML")
include_directories("C:\\SFML\\lib\\cmake\\SFML")
target_link_libraries(3in1 sfml-graphics)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2.5 REQUIRED graphics network audio window system)
