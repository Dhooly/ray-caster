cmake_minimum_required(VERSION 3.10)
project(SFML_Lesson)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(SFML_Lesson main.cpp)
target_link_libraries(SFML_Lesson sfml-graphics sfml-window sfml-system)
