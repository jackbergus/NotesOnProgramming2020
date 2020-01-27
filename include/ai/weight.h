//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_WEIGHT_H
#define TUTORIALS_WEIGHT_H

#include <random>
#include <ostream>

/**
 * A weight defines the
 */
struct weight {
    double previous;
    double current;

    /**
     * Using a random engine to initialize the value
     * @param re    random engine
     */
    weight(std::default_random_engine& re);

    /**
     * Using a default value to initialize the weight
     * @param val
     */
    weight(double val);

    /**
     * Using the system rand to initialize the weight
     */
    weight();

    void reset();

    void reset(std::default_random_engine &re);

    friend std::ostream &operator<<(std::ostream &os, const weight &weight);
};



#endif //TUTORIALS_WEIGHT_H
