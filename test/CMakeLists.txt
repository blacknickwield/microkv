cmake_minimum_required(VERSION 3.22)

project(microkv-test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(LRUTest cache/LRUTest.cpp)
target_link_libraries(LRUTest GTest::gtest_main)
target_include_directories(LRUTest PUBLIC ../include)
include(GoogleTest)
gtest_add_tests(TARGET LRUTest)

add_executable(FileWriterTest file/FileWriterTest.cpp ../src/file/FileWriter.cpp)
target_link_libraries(FileWriterTest GTest::gtest_main)
target_include_directories(FileWriterTest PUBLIC ../include)
include(GoogleTest)
gtest_add_tests(TARGET FileWriterTest)

add_executable(SkipListTest memory/SkipListTest.cpp ../src/memory/Allocator.cpp)
target_link_libraries(SkipListTest GTest::gtest_main)
target_include_directories(SkipListTest PUBLIC ../include)
include(GoogleTest)
gtest_add_tests(TARGET SkipListTest)

add_executable(AllocatorTest memory/AllocatorTest.cpp  ../src/memory/Allocator.cpp)
target_link_libraries(AllocatorTest GTest::gtest_main)
target_include_directories(AllocatorTest PUBLIC ../include)
include(GoogleTest)
gtest_add_tests(TARGET AllocatorTest)