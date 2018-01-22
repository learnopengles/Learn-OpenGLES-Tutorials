# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")


add_library(
                # Sets the name of the library.
                lesson-lib

                # Sets the library as a shared library.
                SHARED

                # graphics
                src/main/cpp/graphics/GLUtils.h
                src/main/cpp/graphics/GLUtils.cpp
                src/main/cpp/graphics/Matrix.h
                src/main/cpp/graphics/Matrix.cpp

                # Provides a relative path to your source file(s).
                # lesson1
                src/main/cpp/lesson1/Native1Lesson.cpp
                src/main/cpp/lesson1/Native1Lesson.h

                # lesson2
                src/main/cpp/lesson2/Native2Lesson.cpp
                src/main/cpp/lesson2/Native2Lesson.h

                # lesson3
                src/main/cpp/lesson3/Native3Lesson.cpp
                src/main/cpp/lesson3/Native3Lesson.h

                # lesson4
                src/main/cpp/lesson4/Native4Lesson.cpp
                src/main/cpp/lesson4/Native4Lesson.h

                # lesson5
                src/main/cpp/lesson5/Native5Lesson.cpp
                src/main/cpp/lesson5/Native5Lesson.h

                # lesson6
                src/main/cpp/lesson6/Native6Lesson.cpp
                src/main/cpp/lesson6/Native6Lesson.h

                # lesson7
                src/main/cpp/lesson7/Native7Lesson.cpp
                src/main/cpp/lesson7/Native7Lesson.h
                src/main/cpp/lesson7/Cubes.cpp
                src/main/cpp/lesson7/Cubes.h
                src/main/cpp/lesson7/CubesClientSide.cpp
                src/main/cpp/lesson7/CubesClientSide.h
                src/main/cpp/lesson7/CubesClientSideWithStride.cpp
                src/main/cpp/lesson7/CubesClientSideWithStride.h
                src/main/cpp/lesson7/CubesWithVbo.cpp
                src/main/cpp/lesson7/CubesWithVbo.h
                src/main/cpp/lesson7/CubesWithVboWithStride.cpp
                src/main/cpp/lesson7/CubesWithVboWithStride.h
                src/main/cpp/lesson7/GenData.cpp
                src/main/cpp/lesson7/GenData.h

                # lesson8
                src/main/cpp/lesson8/Native8Lesson.cpp
                src/main/cpp/lesson8/Native8Lesson.h
                src/main/cpp/lesson8/HeightMap.cpp
                src/main/cpp/lesson8/HeightMap.h
             )

include_directories(src/main/cpp/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       lesson-lib
                       EGL
                       GLESv2
                       android
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )