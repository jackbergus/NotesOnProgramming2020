//
// Created by giacomo on 28/01/2020.
//

#include "ai/datasets/StarcraftAllDimensions.h"

bool dataset_full_dimensions::operator()(const StarcraftReplayDataset &lhs, const StarcraftReplayDataset &rhs) const {
    return std::tie(lhs.GameId, lhs.MaxTimeStamp, lhs.league_index, lhs.age, lhs.hours_per_week, lhs.total_hours, lhs.APM, lhs.SelectByHotkeys,
                    lhs.AssignToHotkeys, lhs.UniqueHotkeys, lhs.MinimapAttacks, lhs.MinimapRightClicks, lhs.NumberOfPACs,
                    lhs.GapBetweenPACs, lhs.ActionLatency, lhs.ActionsInPAC, lhs.TotalMapExplored, lhs.WorkersMade, lhs.UniqueUnitsMade,
                    lhs.ComplexUnitsMade, lhs.ComplexAbilityUsed) ==
           std::tie(rhs.GameId, rhs.MaxTimeStamp, rhs.league_index, rhs.age, rhs.hours_per_week, rhs.total_hours,
                    rhs.APM, rhs.SelectByHotkeys, rhs.AssignToHotkeys, rhs.UniqueHotkeys, rhs.MinimapAttacks,
                    rhs.MinimapRightClicks, rhs.NumberOfPACs, rhs.GapBetweenPACs, rhs.ActionLatency,
                    rhs.ActionsInPAC, rhs.TotalMapExplored, rhs.WorkersMade, rhs.UniqueUnitsMade,
                    rhs.ComplexUnitsMade, rhs.ComplexAbilityUsed);
}

size_t dataset_full_dimensions::operator()(const StarcraftReplayDataset &str) const {
    return multihash(str.age, str.hours_per_week, str.total_hours, str.APM, str.SelectByHotkeys, str.AssignToHotkeys,
                     str.UniqueHotkeys, str.MinimapAttacks,str. MinimapRightClicks, str.NumberOfPACs, str.GapBetweenPACs,
                     str.ActionLatency, str.ActionsInPAC, str.TotalMapExplored, str.WorkersMade, str.UniqueUnitsMade,
                     str.ComplexUnitsMade, str.ComplexAbilityUsed, str.MaxTimeStamp);
}

void dataset_full_dimensions::fit_sample(std::vector<dlib::matrix<double>> &dataset, std::vector<double> &classes,
                                         const StarcraftReplayDataset &x) const {
    dlib::matrix<double> sample(dimensions, 1);
    sample(0) = x.age / 44.0;
    sample(1) = x.hours_per_week / 169.0;
    sample(2) = x.total_hours / 1.0e+06;
    sample(3) = x.APM / 390.0;
    sample(4) = x.SelectByHotkeys;
    sample(5) = x.AssignToHotkeys;
    sample(6) = x.UniqueHotkeys;
    sample(7) = x.MinimapAttacks;
    sample(8) = x.MinimapRightClicks;
    sample(9) = x.NumberOfPACs;
    sample(10) = x.GapBetweenPACs / 238.0;
    sample(11) = x.ActionLatency / 177.0;
    sample(12) = x.ActionsInPAC / 186.0;
    sample(13) = x.TotalMapExplored;
    sample(14) = x.WorkersMade;
    sample(15) = x.UniqueUnitsMade;
    sample(16) = x.ComplexUnitsMade;
    sample(17) = x.ComplexAbilityUsed;
    sample(18) = x.MaxTimeStamp / 389000.0;
    assert(max(sample) <= 1.0); // Checking that all the data inputs are in [0,1]
    dataset.emplace_back(sample);
    classes.emplace_back(x.league_index);
}

void dataset_full_dimensions::fit_output(std::vector<dlib::matrix<double>> &dataset, const StarcraftReplayDataset &x,
                                         size_t max_classes) {
    dlib::matrix<double> sample(max_classes+1, 1);
    const auto& current = x.league_index;
    for (size_t i = 0; i<=max_classes; i++) {
        sample(i) = (i == current) ? 1 : 0;
    }
    assert(max(sample) == 1.0);
    dataset.emplace_back(sample);
}
