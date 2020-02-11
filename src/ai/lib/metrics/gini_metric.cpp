//
// Created by giacomo on 29/01/2020.
//

#include "ai/metrics/gini_metric.h"

#include <cmath>

double gini_metric::metric_root(double pos, double neg) const { return 1.0 - std::pow(pos, 2) - std::pow(neg, 2); }

double gini_metric::subtree_part(double freq_posLeft) { return 1.0 - std::pow(freq_posLeft, 2) - std::pow(1.0 - freq_posLeft, 2); }

double gini_metric::posterior(double sizeLeft, double sizeTotal, double posLeft, double posRight) {
    double sizeRight = sizeTotal - sizeLeft;
    return (sizeLeft / sizeTotal) * subtree_part(posLeft) + (sizeRight / sizeTotal) * subtree_part(posRight);
}
