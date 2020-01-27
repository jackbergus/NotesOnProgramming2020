//
// Created by giacomo on 24/01/2020.
//

#include "utils/FiniteFunction.h"

finite_function_argument::finite_function_argument(double expectation, std::vector<double> value) : input{std::move(value)} {
    output.emplace_back(expectation);
}

struct finite_function &finite_function::extendMap(double expectation, std::vector<double> value) {
    finite_function.emplace_back(expectation, value);
    return *this;
}
