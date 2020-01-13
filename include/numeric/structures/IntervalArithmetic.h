//
// Created by giacomo on 10/01/2020.
//

#ifndef TUTORIALS_INTERVALARITHMETIC_H
#define TUTORIALS_INTERVALARITHMETIC_H


#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <ostream>

struct IntervalArithmetic {
    float x, y;
    /**
     * Defining the interval [x, y]
     * @param x         minimum value
     * @param y         maximum value
     */
    IntervalArithmetic(float x, float y);

    /**
     * Defining the interval over one single point
     * @param c
     */
    IntervalArithmetic(float c);

    IntervalArithmetic &operator+=(const IntervalArithmetic &i);
    IntervalArithmetic &operator-=(const IntervalArithmetic &i);
    IntervalArithmetic &operator*=(const IntervalArithmetic &i);
    IntervalArithmetic &operator/=(const IntervalArithmetic &i);
    IntervalArithmetic operator+(const IntervalArithmetic &i) const;
    IntervalArithmetic operator-(const IntervalArithmetic &i) const;
    IntervalArithmetic operator*(const IntervalArithmetic &i) const;
    IntervalArithmetic operator/(const IntervalArithmetic &i) const;
    IntervalArithmetic pow(unsigned int pow);
    friend std::ostream &operator<<(std::ostream &os, const IntervalArithmetic &arithmetic);
};

IntervalArithmetic fprime(const IntervalArithmetic& x);

IntervalArithmetic fsecond(const IntervalArithmetic& x);

void examples_on_interval_arithmetic();

#endif //TUTORIALS_INTERVALARITHMETIC_H
