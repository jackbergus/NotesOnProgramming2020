//
// Created by giacomo on 28/01/2020.
//

#include "ai/metrics/class_prediction_distance.h"


double class_prediction_distance(const dlib::matrix<double> &m, const unsigned long expected) {
    double maxValue = -std::numeric_limits<double>::max();
    std::set<unsigned long> candidates;
    for (size_t i = 0, N = m.size(); i<N; i++) {
        if (maxValue < m(i)) {
            maxValue = m(i);
            candidates.clear();
            candidates.emplace(i);
        } else if (maxValue == m(i)) {
            candidates.emplace(i);
        }
    }
    return class_prediction_distance(expected, candidates);

}


