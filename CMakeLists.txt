cmake_minimum_required(VERSION 3.15)
project(tutorials)
include_directories(include)
include_directories(submodules/libfort/lib)
include_directories(submodules/glm/glm)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(submodules/dlib)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/libfort)

add_executable(fibonacci_benchmark src/big_o_notation/lib/fibonacci_no_master_theorem.cpp include/utils/y_combinator.h src/big_o_notation/fibonacci_benchmark.cpp include/big_o_notation/vptree_node.h)

add_executable(vp_tree src/big_o_notation/vantage_point_trees.cpp)
target_link_libraries(vp_tree m)

add_executable(numeric_error src/big_o_notation/lib/fibonacci_no_master_theorem.cpp src/numeric/numeric_error.cpp src/numeric/structures/IntervalArithmetic.cpp include/numeric/structures/IntervalArithmetic.h src/numeric/structures/Sphere.cpp include/numeric/structures/Sphere.h src/numeric/structures/AABB.cpp include/numeric/structures/AABB.h src/numeric/structures/Vector.cpp include/numeric/structures/Vector.h src/utils/float.cpp src/numeric/lib/FibonacciTotalError.cpp src/numeric/structures/OBB.cpp include/numeric/structures/OBB.h)
target_link_libraries(numeric_error fort glm::glm)