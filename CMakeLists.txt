cmake_minimum_required(VERSION 3.10)
project(arena-allocator-cpp)
#Using modern c++ standards

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(test_arena tests/test_arena.cpp)
add_executable(test_pool tests/test_pool.cpp)
add_executable(test_allocator tests/test_allocator.cpp)
add_executable(benchmark tests/benchmark.cpp)

target_compile_options(test_arena PRIVATE -fsanitize=address,undefined)
target_link_options(test_arena PRIVATE -fsanitize=address,undefined)
target_compile_options(test_pool PRIVATE -fsanitize=address,undefined)
target_link_options(test_pool PRIVATE -fsanitize=address,undefined)
target_compile_options(test_allocator PRIVATE -fsanitize=address,undefined)
target_link_options(test_allocator PRIVATE -fsanitize=address,undefined)