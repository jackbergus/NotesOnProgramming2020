//
// Created by giacomo on 28/01/2020.
//

#include "ai/datasets/Starcraft9Dimensions.h"

bool dataset_9_dimensions::operator()(const StarcraftReplayDataset &str1, const StarcraftReplayDataset &str2) const {
    return std::tie(str1.APM, str1.SelectByHotkeys,
                    str1.AssignToHotkeys, str1.UniqueHotkeys, str1.MinimapAttacks, str1.MinimapRightClicks, str1.NumberOfPACs,
                    str1.GapBetweenPACs, str1.ActionLatency) ==
           std::tie(str2.APM, str2.SelectByHotkeys,
                    str2.AssignToHotkeys, str2.UniqueHotkeys, str2.MinimapAttacks, str2.MinimapRightClicks, str2.NumberOfPACs,
                    str2.GapBetweenPACs, str2.ActionLatency);
}

size_t dataset_9_dimensions::operator()(const StarcraftReplayDataset &str) const {
    return multihash(str.APM, str.SelectByHotkeys, str.AssignToHotkeys, str.UniqueHotkeys, str.MinimapAttacks,
                     str. MinimapRightClicks, str.NumberOfPACs, str.GapBetweenPACs, str.ActionLatency);
}

void dataset_9_dimensions::fit_sample(std::vector<dlib::matrix<double>> &dataset, std::vector<double> &classes,
                                      const StarcraftReplayDataset &x) {
    dlib::matrix<double> sample(dimensions, 1);
    sample(0) = x.APM / 390.0;
    sample(1) = x.SelectByHotkeys;
    sample(2) = x.AssignToHotkeys;
    sample(3) = x.UniqueHotkeys;
    sample(4) = x.MinimapAttacks;
    sample(5) = x.MinimapRightClicks;
    sample(6) = x.NumberOfPACs;
    sample(7) = x.GapBetweenPACs / 238.0;
    sample(8) = x.ActionLatency / 177.0;
    assert(max(sample) <= 1.0); // Checking that all the data inputs are in [0,1]
    dataset.emplace_back(sample);
    classes.emplace_back(x.league_index);
}

void dataset_9_dimensions::fit_output(std::vector<dlib::matrix<double>> &dataset, const StarcraftReplayDataset &x,
                                      size_t max_classes) {
    dlib::matrix<double> sample(max_classes+1, 1);
    const auto& current = x.league_index;
    for (size_t i = 0; i<=max_classes; i++) {
        sample(i) = (i == current) ? 1 : 0;
    }
    assert(max(sample) == 1.0);
    dataset.emplace_back(sample);
}
