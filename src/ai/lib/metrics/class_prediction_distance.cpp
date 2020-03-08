//
// Created by giacomo on 28/01/2020.
//

#include "ai/metrics/class_prediction_distance.h"


double class_prediction_distance(const dlib::matrix<double> &m, const unsigned long expected) {
    double maxValue = -std::numeric_limits<double>::max();
    std::set<unsigned long> candidates; // Returning the caniddates maximizing the \tilde{h} value
    for (size_t i = 0, N = m.size(); i<N; i++) {
        if (maxValue < m(i)) {          // Getting a new maximum value
            maxValue = m(i);
            candidates.clear();
            candidates.emplace(i);      // Adding the new candidate to the cleared set
        } else if (maxValue == m(i)) {
            candidates.emplace(i);      // Adding a candidate with the same size
        }
    }
    return class_prediction_distance(expected, candidates);

}


