//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_STARCRAFT9DIMENSIONS_H
#define TUTORIALS_STARCRAFT9DIMENSIONS_H


#include <ai/datasets/StarcraftReplayDataset.h>
#include <utils/hashing.h>
#include <dlib/matrix.h>

/**
 * Delegating the different equality and hashing functions for the StarcraftReplayDataset struct to different structs.
 * This struct will provide the subdivision of the dataset into 9 dimensions, and will make it fit inside matrices.
 */
struct dataset_9_dimensions {
    static constexpr size_t dimensions = 9;

    bool operator()(const StarcraftReplayDataset & str1, const StarcraftReplayDataset & str2) const;
    size_t operator()(const StarcraftReplayDataset & str) const;
    void fit_sample(std::vector<dlib::matrix<double>>& dataset, std::vector<double>&classes, const StarcraftReplayDataset& x);
    void fit_output(std::vector<dlib::matrix<double>>& dataset, const StarcraftReplayDataset& x, const size_t max_classes = 8);
};

#endif //TUTORIALS_STARCRAFT9DIMENSIONS_H
