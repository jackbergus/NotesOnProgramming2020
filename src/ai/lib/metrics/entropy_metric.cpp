//
// Created by giacomo on 29/01/2020.
//

#include <cmath>
#ifdef  _WIN64
#include <limits>		// MSVC requires limits to have the numeric setup
#endif
#include "ai/metrics/entropy_metric.h"

static inline double xlogeps(double x) {
    static double eps = std::numeric_limits<double>::epsilon();
    return (x <= eps) ? 0.0 : x * std::log2(x);
}

double entropy_metric::metric_root(double posRoot, double negRoot) const { return - posRoot * xlogeps(posRoot) - negRoot * xlogeps(negRoot); }

double entropy_metric::subtree_part(double freq_posLeft) { return - freq_posLeft * xlogeps(freq_posLeft) - (1.0 - freq_posLeft) * xlogeps(1.0 - freq_posLeft); }

double entropy_metric::posterior(double sizeLeft, double sizeTotal, double posLeft, double posRight) {
    return (0.5) * subtree_part(posLeft) + (0.5) * subtree_part(posRight);
}
