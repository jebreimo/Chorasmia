# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-12-27.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(catch
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v2.13.3"
    )
FetchContent_MakeAvailable(catch)

add_executable(ChorasmiaTest
    ChorasmiaTestMain.cpp
    test_Array2D.cpp
    test_ArrayView2D.cpp
    test_ArrayView2DAlgorithms.cpp
    test_Index2D.cpp
    test_IntervalMap.cpp
    test_MutableArrayView2D.cpp
    )

target_link_libraries(ChorasmiaTest
    PRIVATE
        Catch2::Catch2
        Chorasmia::Chorasmia
    )
