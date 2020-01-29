//
// Created by giacomo on 24/01/2020.
//

#include "ai/weight.h"

weight::weight(std::default_random_engine &re) {
    reset(re);
}

weight::weight(double val) {
    current = val;
    previous = 0.0;
}

weight::weight() {
    reset();
}

void weight::reset() {
    current = (((rand() % 2) == 1) ?
                          (-1 * (double(rand()) / (double(RAND_MAX) + 1.0))) :
                          ( double(rand()) / (double(RAND_MAX) + 1.0)));
    previous = 0.0;
}

void weight::reset(std::default_random_engine &re) {
    std::uniform_real_distribution<double> unif(-1, 1);
    current = unif(re);
    previous = 0.0;
}

std::ostream &operator<<(std::ostream &os, const weight &weight) {
    os << "[" << weight.previous << "," << weight.current << "]";
    return os;
}
