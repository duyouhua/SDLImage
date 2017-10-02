# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../src/main/jniLibs)

# SDL核心库(最重要的库)
add_library(
            SDL2
            SHARED
            IMPORTED)
set_target_properties(
            SDL2
            PROPERTIES IMPORTED_LOCATION
            ../../../../src/main/jniLibs/armeabi/libSDL2.so)
add_library(
            SDL2_image
            SHARED
            IMPORTED)
set_target_properties(
            SDL2_image
            PROPERTIES IMPORTED_LOCATION
            ../../../../src/main/jniLibs/armeabi/libSDL2_image.so)
add_library(
            SDL2_mixer
            SHARED
            IMPORTED)
set_target_properties(
            SDL2_mixer
            PROPERTIES IMPORTED_LOCATION
            ../../../../src/main/jniLibs/armeabi/libSDL2_mixer.so)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(src/main/jniLibs/src)
include_directories(src/main/jniLibs/include)
include_directories(src/main/cpp)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             SDL2main

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             #配置SDL程序入口函数
             src/main/jniLibs/src/main/android/SDL_android_main.c
             #src/main/cpp/SDLTest.cpp
             #src/main/cpp/SDL_Lesson.c
             #src/main/cpp/SDL_Test1.cpp
             #src/main/cpp/SDL_Test2.cpp
             #src/main/cpp/SDL_Test3.cpp
             #src/main/cpp/SDL_Test4.cpp
             src/main/cpp/SDL_Test5.cpp
             #src/main/cpp/SDL_Test6.cpp
             #src/main/cpp/SDL_Test7.cpp
             #src/main/cpp/audio.c
             #src/main/cpp/test.c
             #src/main/cpp/playmus.c
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       SDL2main SDL2 SDL2_image SDL2_mixer

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
