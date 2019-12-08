cmake_minimum_required(VERSION 3.15)
project(tutorials)
include_directories(include)
set(CMAKE_CXX_STANDARD 14)

add_executable(fibonacci_benchmark src/big_o_notation/lib/fibonacci_no_master_theorem.cpp include/utils/y_combinator.h src/big_o_notation/fibonacci_benchmark.cpp include/big_o_notation/vptree_node.h)
add_executable(vp_tree src/big_o_notation/vantage_point_trees.cpp)
target_link_libraries(vp_tree m)