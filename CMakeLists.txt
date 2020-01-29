cmake_minimum_required(VERSION 3.14)
## Giving a name to the project
project(tutorials)

## Including headers, from both the current project and from the imported libraries
include_directories(include)
include_directories(submodules/csv)
include_directories(submodules/libfort/lib)
include_directories(submodules/glm/glm)
include_directories(submodules/rang/include)
include_directories(submodules/fpgrowth/include)

## I want to use the C++17 standard, forsooth!
set(CMAKE_CXX_STANDARD 17)

## Including the libraries via subdirectories: in fact, this is the most cross-platform way as you might get now...
add_subdirectory(submodules/dlib)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/libfort)


## Each add_executable provides a binary:
## 1) Benchmark of the fibonacci numbers
add_executable(fibonacci_benchmark src/big_o_notation/lib/fibonacci_no_master_theorem.cpp include/utils/y_combinator.h src/big_o_notation/fibonacci_benchmark.cpp include/big_o_notation/vptree_node.h include/utils/redi.h include/ai/rtree/decision_tree.h)


## 2) Vantage Point Tree (no library is linked)
add_executable(vp_tree src/big_o_notation/vantage_point_trees.cpp)
target_link_libraries(vp_tree)

# 3) Numeric errors: Some algorithms on collision detections. We're going to use the OpenGL Mathematics library
add_executable(numeric_error src/big_o_notation/lib/fibonacci_no_master_theorem.cpp src/numeric/numeric_error.cpp src/numeric/structures/IntervalArithmetic.cpp include/numeric/structures/IntervalArithmetic.h src/numeric/structures/Sphere.cpp include/numeric/structures/Sphere.h src/numeric/structures/AABB.cpp include/numeric/structures/AABB.h src/numeric/structures/Vector.cpp include/numeric/structures/Vector.h src/utils/float.cpp src/numeric/lib/FibonacciTotalError.cpp src/numeric/structures/OBB.cpp include/numeric/structures/OBB.h)
target_link_libraries(numeric_error fort glm::glm)


# 4) Seeing some preliminary learning algorithms
add_executable(learning_1 src/ai/learning.cpp src/ai/lib/mlp/weight.cpp include/ai/mlp/weight.h src/utils/FiniteFunction.cpp include/utils/FiniteFunction.h src/ai/lib/mlp/Perceptron.cpp src/ai/lib/mlp/Layer.cpp include/ai/mlp/Layer.h src/ai/lib/mlp/BackwardPropagationNetwork.cpp include/ai/mlp/BackwardPropagationNetwork.h src/ai/lib/mlp/SinglePerceptron.cpp include/ai/mlp/SinglePerceptron.h src/ai/lib/SupportVectorMachines.cpp include/ai/SupportVectorMachines.h src/utils/permutation.cpp include/utils/permutation.h src/ai/lib/datasets/StarcraftReplayDataset.cpp include/ai/datasets/StarcraftReplayDataset.h src/utils/hashing.cpp include/utils/hashing.h src/ai/lib/datasets/Starcraft9Dimensions.cpp include/ai/datasets/Starcraft9Dimensions.h src/ai/lib/datasets/StarcraftAllDimensions.cpp include/ai/datasets/StarcraftAllDimensions.h include/ai/datasets/training_testing_sets.h src/ai/lib/datasets/DLibSplits.cpp include/ai/datasets/DLibSplits.h src/ai/lib/metrics/class_prediction_distance.cpp include/ai/metrics/class_prediction_distance.h src/ai/lib/rtree/split_field.cpp include/ai/rtree/split_field.h src/ai/lib/metrics/gini_metric.cpp include/ai/metrics/gini_metric.h src/ai/lib/metrics/entropy_metric.cpp include/ai/metrics/entropy_metric.h)
target_link_libraries(learning_1 dlib)

# 5) Last, seeing some data mining algorithms
## In order to do so, I need to find the Thread library in a cross platform way
find_package(Threads REQUIRED)
add_executable(data_mining_2 src/dm/data_mining.cpp src/dm/lib/dataset/DeckItem.cpp include/dm/dataset/DeckItem.h src/dm/lib/dataset/ClashRoyalePlayer.cpp include/dm/dataset/ClashRoyalePlayer.h src/dm/lib/dataset/ClashRoyaleDatasetMatch.cpp include/dm/dataset/ClashRoyaleDatasetMatch.h)
target_link_libraries(data_mining_2 Threads::Threads)