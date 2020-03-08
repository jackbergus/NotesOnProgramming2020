//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_WEIGHT_H
#define TUTORIALS_WEIGHT_H

#include <random>
#include <ostream>

/**
 * A weight structure contains both the current weight value and the one obtained in the previous training step
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

    /**
     * Resetting the weight using the C default random number generator
     */
    void reset();

    /**
     * Resetting the weight using a C++11 random number generator
     * @param re
     */
    void reset(std::default_random_engine &re);

    friend std::ostream &operator<<(std::ostream &os, const weight &weight);
};



#endif //TUTORIALS_WEIGHT_H
