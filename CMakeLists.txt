cmake_minimum_required(VERSION 3.22)

project(microkv)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB_RECURSE SRC
        src/filter/*.cpp
        src/memory/*.cpp
        src/db/*.cpp
        src/cache/*.cpp
        src/file/*.cpp
        src/sstable/*.cpp
        src/util/*.cpp)

add_executable(main ${SRC})
# include_directories(main PUBLIC include)