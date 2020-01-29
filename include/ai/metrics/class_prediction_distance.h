//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_CLASS_PREDICTION_DISTANCE_H
#define TUTORIALS_CLASS_PREDICTION_DISTANCE_H

#include <dlib/matrix.h>
#include <set>

double class_prediction_distance(const dlib::matrix<double>& m, const unsigned long expected);

template <typename T>
double class_prediction_distance(const T expected,
                                 std::set<T> &candidates) {//assert(candidates.size() <= 1);

    // Getting the average distance between all the candidates and the expected label
    double avgPredictionSimilarity = 0.0;

    // a) first, evaluate the average distance between the componetns
    for (auto it = candidates.begin(); it != candidates.cend(); it++) {
        // evaluate the distance between expected and predicted label
        double tmpDistance = std::abs(((double)*it) - (double)expected);
        // sum the normalized distance
        avgPredictionSimilarity += (tmpDistance / (tmpDistance + 1));
    }
    //std::cout << " vs " << expected << std::endl;
    if (!candidates.empty()) avgPredictionSimilarity /= ((double)candidates.size());
    // b) then, invert it so it becomes a similarity
    avgPredictionSimilarity = 1.0 - avgPredictionSimilarity;

#define WEIGHTED_DISTANCE
#ifdef WEIGHTED_DISTANCE
    if (candidates.empty())
        return 1.0;             // If no candidates were returned, then I have the maximum error
    if (candidates.find(expected) != candidates.cend()) {
        if (candidates.size() == 1)
            return 0.0;        // If the precision was maximum, then the error was minimum
        else {
            // The distance shall be the number of the wrong candidates over
            double tmp =  1.0 - ((avgPredictionSimilarity * (1.0 - ((double)candidates.size() - 1.0) / ((double)candidates.size()))));
            //// assert((tmp >= 0) && (tmp <= 1.0)); /// sanity check: the normalized distance shall be in [0,1]
            return tmp;
        }
    }  else {
        double tmp = 1.0 - ((avgPredictionSimilarity * (1.0 - ((double)candidates.size()) / ((double)candidates.size() + 1.0))));
        //// assert((tmp >= 0) && (tmp <= 1.0)); /// sanity check: the normalized distance shall be in [0,1]
        return tmp;
    }
#else
    return avgPredictionDistance;
#endif
}

#endif //TUTORIALS_CLASS_PREDICTION_DISTANCE_H
