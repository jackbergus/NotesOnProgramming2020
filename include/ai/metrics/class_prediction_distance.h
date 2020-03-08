//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_CLASS_PREDICTION_DISTANCE_H
#define TUTORIALS_CLASS_PREDICTION_DISTANCE_H

#include <dlib/matrix.h>
#include <set>

double class_prediction_distance(const dlib::matrix<double> &m, const unsigned long expected);

template <typename T>
double class_prediction_distance(const T expected,
                                 std::set<T> &candidates) {//assert(candidates.size() <= 1);

    // Getting the average distance between all the candidates and the expected label
    double avgPredictionSimilarity = 0.0;

    // a) first, evaluate the average distance between the components
    for (auto it = candidates.begin(); it != candidates.cend(); it++) {
        // evaluate the distance between expected and predicted label
        double tmpDistance = std::abs(((double)*it) - (double)expected);
        // sum the normalized distance
        avgPredictionSimilarity += (tmpDistance / (tmpDistance + 1));
    }
    if (!candidates.empty()) avgPredictionSimilarity /= ((double)candidates.size());
    // b) then, invert it so it becomes a similarity
    avgPredictionSimilarity = 1.0 - avgPredictionSimilarity;

    if (candidates.empty())
        return 1.0;             // If no candidates were returned, then I have the maximum error
    if (candidates.find(expected) != candidates.cend()) {
        if (candidates.size() == 1)
            return 0.0;         // If the precision was maximum, then the error was minimum
        else {
            // The distance shall be weighted by the number of wrong candidates that we get (all the candidates minus the correclty predicted one)
            return 1.0 - ((avgPredictionSimilarity * (1.0 - ((double)candidates.size() - 1.0) / ((double)candidates.size()))));
        }
    }  else {
        // The distance shall be weighted by the number of wrong candidates that we get (all the predicted candidates)
        return 1.0 - ((avgPredictionSimilarity * (1.0 - ((double)candidates.size()) / ((double)candidates.size() + 1.0))));
    }
}

#endif //TUTORIALS_CLASS_PREDICTION_DISTANCE_H
