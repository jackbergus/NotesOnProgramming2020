cmake_minimum_required(VERSION 3.14)
project(tutorials)
include_directories(include)
include_directories(submodules/csv)
include_directories(submodules/libfort/lib)
include_directories(submodules/glm/glm)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(submodules/dlib)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/libfort)

add_executable(fibonacci_benchmark src/big_o_notation/lib/fibonacci_no_master_theorem.cpp include/utils/y_combinator.h src/big_o_notation/fibonacci_benchmark.cpp include/big_o_notation/vptree_node.h include/utils/redi.h)

add_executable(vp_tree src/big_o_notation/vantage_point_trees.cpp)
target_link_libraries(vp_tree)

add_executable(numeric_error src/big_o_notation/lib/fibonacci_no_master_theorem.cpp src/numeric/numeric_error.cpp src/numeric/structures/IntervalArithmetic.cpp include/numeric/structures/IntervalArithmetic.h src/numeric/structures/Sphere.cpp include/numeric/structures/Sphere.h src/numeric/structures/AABB.cpp include/numeric/structures/AABB.h src/numeric/structures/Vector.cpp include/numeric/structures/Vector.h src/utils/float.cpp src/numeric/lib/FibonacciTotalError.cpp src/numeric/structures/OBB.cpp include/numeric/structures/OBB.h)
target_link_libraries(numeric_error fort glm::glm)

add_executable(learning_1 main.cpp src/ai/weight.cpp include/ai/weight.h src/utils/FiniteFunction.cpp include/utils/FiniteFunction.h src/ai/Perceptron.cpp src/ai/Layer.cpp include/ai/Layer.h src/ai/BackwardPropagationNetwork.cpp include/ai/BackwardPropagationNetwork.h src/ai/SinglePerceptron.cpp include/ai/SinglePerceptron.h src/ai/SupportVectorMachines.cpp include/ai/SupportVectorMachines.h EntropyTree.cpp EntropyTree.h src/utils/permutation.cpp include/utils/permutation.h src/ai/datasets/StarcraftReplayDataset.cpp include/ai/datasets/StarcraftReplayDataset.h src/utils/hashing.cpp include/utils/hashing.h src/ai/datasets/Starcraft9Dimensions.cpp include/ai/datasets/Starcraft9Dimensions.h src/ai/datasets/StarcraftAllDimensions.cpp include/ai/datasets/StarcraftAllDimensions.h include/ai/datasets/training_testing_sets.h src/ai/datasets/DLibSplits.cpp include/ai/datasets/DLibSplits.h class_prediction_distance.cpp class_prediction_distance.h)
target_link_libraries(learning_1 dlib)