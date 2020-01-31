//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_TRAINING_TESTING_SETS_H
#define TUTORIALS_TRAINING_TESTING_SETS_H

#include <unordered_set>
#include <iostream>
#include <csv.h>
#include <utils/numeric/permutation.h>
#include "DLibSplits.h"
#include "StarcraftReplayDataset.h"

/**
 * Utility struct providing a splitting between training and test data set from a set of unique elements.
 *
 * @tparam T            Type of the elements that need to be sorted
 * @tparam Hash         Hashing function associated to the elements
 * @tparam Eq           Equality function associated to the elements
 */
template <typename T, typename  HashEq>
struct training_testing_sets {
    std::unordered_set<T, HashEq, HashEq> training, testing;

    training_testing_sets(std::unordered_set<T, HashEq, HashEq>& originalSet, const double percentage = 0.7) {
        double i = 0, N = originalSet.size();
        for (auto it = originalSet.begin(); it != originalSet.cend(); it++) {
            (((i)/N < percentage) ? training : testing).emplace(*it);
            i += 1;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const training_testing_sets &sets) {
        double Tr = sets.training.size();
        double Ts = sets.testing.size();
        os << "training: " << (Tr)/(Tr+Ts) << " testing: " << (Ts)/(Tr+Ts);
        return os;
    }
};



template <typename HashEq>
std::pair<const size_t, const size_t>
generateSplit(const std::string &datasetName, DLib_Splits &out, bool normalize = true, const double trainingSize = 0.7) {
    // Reading the StarCraft dataset
    io::CSVReader<21> starcraft{datasetName};
    HashEq dlibMover;
    starcraft.read_header(io::ignore_extra_column, "GameID", "LeagueIndex", "Age", "HoursPerWeek", "TotalHours", "APM", "SelectByHotkeys", "AssignToHotkeys", "UniqueHotkeys", "MinimapAttacks", "MinimapRightClicks", "NumberOfPACs", "GapBetweenPACs", "ActionLatency", "ActionsInPAC", "TotalMapExplored", "WorkersMade", "UniqueUnitsMade", "ComplexUnitsMade", "ComplexAbilityUsed", "MaxTimeStamp");

    // Used for the table name information
    dlibMover.set_label_names(out.colnames);

    // This final map will contain both the training and the testing dataset
    // We will split the dataset into training and testing for each of the classes
    std::unordered_map<unsigned short, training_testing_sets<StarcraftReplayDataset, HashEq>> splitting_map;
    {
        // Reading all the datasets: using the custom hashing and equality functions, so that we automatically project the data as we wish
        std::cout << "Reading the CSV file" << std::endl;
        std::unordered_map<unsigned short, std::unordered_set<StarcraftReplayDataset, HashEq, HashEq>> map;
        bool continueReading = true;
        do {
            auto x = StarcraftReplayDataset::read(starcraft);
            if (x) {                                                // If it was possible to read the information from the file
                map[x->league_index].emplace(x.value());       // Insert it inside the map
            } else {
                continueReading = false;                            // Otherwise, fail and stop reading
            }
        } while (continueReading);

        // Defining the split size for the training/testing dataset. Inserting those after removing the duplicates.
        std::cout << "Splitting everything in human-readable training and testing sets" << std::endl;
        for (auto it = map.begin(); it != map.cend(); it++) {
            // Splitting the previously loaded set of data into the training and the testing dataset
            splitting_map.insert(std::make_pair(it->first, training_testing_sets<StarcraftReplayDataset, HashEq>{it->second, trainingSize}));
        }
    }

    // Preparing the loaded data in the format that DLib expects.
    // Please note that this multiple-step implementation is just to be debuggable and to be sure what we're doing,
    // but everything can be done more efficiently in one single step.
    std::cout << "Copying the information for a DLib-compatible representation" << std::endl;
    for (auto it = splitting_map.begin(); it != splitting_map.cend(); it++) {
        const training_testing_sets<StarcraftReplayDataset, HashEq>& splits = it->second;

        for (const StarcraftReplayDataset& trainingDatum : splits.training) {
            dlibMover.fit_sample(out.training_input, out.training_labels, trainingDatum, normalize);
            dlibMover.fit_output(out.training_label_vector, trainingDatum, splitting_map.size());
        }

        for (const StarcraftReplayDataset& testingDatum : splits.testing) {
            dlibMover.fit_sample(out.testing_input, out.testing_labels, testingDatum, normalize);
            dlibMover.fit_output(out.testing_label_vector, testingDatum, splitting_map.size());
        }
        std::cout << it->first << "~" << it->second << std::endl;
    }

    // Shuffling the data
    std::cout << "Performing a random data permutation" << std::endl;
    std::vector<int> permutation = generateRandomPermutation(out.training_input.size());
    out.training_input = permute(out.training_input, permutation);
    out.training_labels = permute(out.training_labels, permutation);
    out.training_label_vector = permute(out.training_label_vector, permutation);

    // <number of classes, number of dimensions == mlp input>
    std::cout << "Dataset ordering done" << std::endl;
    out.dimension = dlibMover.dimensions;
    return {splitting_map.size(), dlibMover.dimensions};
}

#endif //TUTORIALS_TRAINING_TESTING_SETS_H
