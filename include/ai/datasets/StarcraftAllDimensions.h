//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_STARCRAFTALLDIMENSIONS_H
#define TUTORIALS_STARCRAFTALLDIMENSIONS_H


#include <ai/datasets/StarcraftReplayDataset.h>
#include <utils/hashing.h>
#include <dlib/matrix.h>

struct dataset_full_dimensions {
    static constexpr size_t dimensions = 19;

    bool operator()(const StarcraftReplayDataset & lhs, const StarcraftReplayDataset & rhs) const;
    size_t operator()(const StarcraftReplayDataset & str) const;
    void fit_sample(std::vector<dlib::matrix<double>>& dataset, std::vector<double>&classes, const StarcraftReplayDataset& x) const;
    void fit_output(std::vector<dlib::matrix<double>>& dataset, const StarcraftReplayDataset& x, const size_t max_classes = 8);
};


#endif //TUTORIALS_STARCRAFTALLDIMENSIONS_H
