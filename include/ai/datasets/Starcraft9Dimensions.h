//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_STARCRAFT9DIMENSIONS_H
#define TUTORIALS_STARCRAFT9DIMENSIONS_H


#include <ai/datasets/StarcraftReplayDataset.h>
#include <utils/numeric/hashing.h>
#include <dlib/matrix.h>

/**
 * Delegating the different equality and hashing functions for the StarcraftReplayDataset struct to different structs.
 * This struct will provide the subdivision of the dataset into 9 dimensions, and will make it fit inside matrices.
 */
struct dataset_9_dimensions {
    static constexpr size_t dimensions = 9;

    /**
     * Checking whether the two components are the same
     * @param str1  Left hand side
     * @param str2  Right hand side
     * @return      Comparison output
     */
    bool operator()(const StarcraftReplayDataset & str1, const StarcraftReplayDataset & str2) const;

    /**
     * Providing a hashing to the Starcraft data point
     * @param str   The data point to be hashed
     * @return      The associated hashing value
     */
    size_t operator()(const StarcraftReplayDataset & str) const;

    /**
     * Initializes some components that will be part of the DLib_Split class (1)
     * @param dataset       Input vectorial representation
     * @param classes       Single value class representation (expected outcome)
     * @param x             Data point from which extract the representation
     * @param normalize     Whether the values should be normalized (e.g., for Multilayer Perceptron Networks, and not necessairly for SVMs)
     */
    void fit_sample(std::vector<dlib::matrix<double>> &dataset, std::vector<double> &classes, const StarcraftReplayDataset &x, bool normalize = true);

    /**
     * Initializes some components that will be part of the DLib_Split class (2)
     * @param dataset \     Vectorial representation of the expected output of the classification outcome as a vector
     * @param x             Data point from which extract the representation
     * @param max_classes   How many classes are we expect to provide as an output
     */
    void fit_output(std::vector<dlib::matrix<double>>& dataset, const StarcraftReplayDataset& x, const size_t max_classes = 8);

    /**
     * Returns the dimensions that this projection will represent
     * @param schema_name
     */
    void set_label_names(std::vector<std::string>& schema_name);
};

#endif //TUTORIALS_STARCRAFT9DIMENSIONS_H
