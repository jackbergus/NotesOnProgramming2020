//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_FINITEFUNCTION_H
#define TUTORIALS_FINITEFUNCTION_H

#include <utility>
#include <vector>

struct finite_function_argument {
    std::vector<double> input;
    std::vector<double> output;

    finite_function_argument(double expectation, std::vector<double> value);
};

struct finite_function {
    std::vector<finite_function_argument> finite_function;

    struct finite_function& extendMap(double expectation, std::vector<double> value);
};


#endif //TUTORIALS_FINITEFUNCTION_H
