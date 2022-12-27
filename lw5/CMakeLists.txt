cmake_minimum_required(VERSION 3.22)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_library(lib1 STATIC lib1.c lib.h)
add_library(lib2 STATIC lib2.c lib.h)
add_executable(main1 main1.c)
target_link_libraries(main1 lib1 lib2 m)

add_library(lib1d SHARED lib1.c lib.h)
add_library(lib2d SHARED lib2.c lib.h)
add_executable(main2 main2.c)
target_link_libraries(lib1d m)
target_link_libraries(lib2d m)
target_link_libraries(main2 ${CMAKE_DL_LIBS})
