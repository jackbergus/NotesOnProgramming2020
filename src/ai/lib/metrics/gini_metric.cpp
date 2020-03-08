//
// Created by giacomo on 29/01/2020.
//

#include "ai/metrics/gini_metric.h"

#include <cmath>

double gini_metric::node_iota(double freq_posLeft) { return 1.0 - std::pow(freq_posLeft, 2) - std::pow(1.0 - freq_posLeft, 2); }

double gini_metric::posterior(double sizeLeft, double sizeTotal, double posLeft, double posRight) {
    double sizeRight = sizeTotal - sizeLeft;
    return (sizeLeft / sizeTotal) * node_iota(posLeft) + (sizeRight / sizeTotal) * node_iota(posRight);
}
