//
// Created by giacomo on 10/01/2020.
//

#include <iostream>
#include "numeric/structures/IntervalArithmetic.h"


IntervalArithmetic::IntervalArithmetic(float x, float y) : x(std::min(x,y)), y(std::max(x,y)) {}

IntervalArithmetic::IntervalArithmetic(float c) : x{c}, y{c} {}


IntervalArithmetic IntervalArithmetic::operator+(const IntervalArithmetic &i) const {
    IntervalArithmetic result = *this;
    result += i;
    return result;
}

IntervalArithmetic &IntervalArithmetic::operator+=(const IntervalArithmetic &i) {
    x += i.x;
    y += i.y;
    return *this;
}

IntervalArithmetic &IntervalArithmetic::operator-=(const IntervalArithmetic &i) {
    x -= i.x;
    y -= i.y;
    return *this;
}

 IntervalArithmetic IntervalArithmetic::operator-(const IntervalArithmetic &i) const {
    IntervalArithmetic result = *this;
    result -= i;
    return result;
}

IntervalArithmetic IntervalArithmetic::operator*(const IntervalArithmetic &i) const {
    IntervalArithmetic result = *this;
    result *= i;
    return result;
}

IntervalArithmetic IntervalArithmetic::operator/(const IntervalArithmetic &i) const {
    IntervalArithmetic result = *this;
    result /= i;
    return result;
}

IntervalArithmetic &IntervalArithmetic::operator*=(const IntervalArithmetic &i) {
    x = std::min({x*i.x, x*i.y, y*i.x, y*i.y});
    y = std::max({x*i.x, x*i.y, y*i.x, y*i.y});
    return *this;
}

IntervalArithmetic &IntervalArithmetic::operator/=(const IntervalArithmetic &i) {
    if ((i.x <= 0) && (0 <= i.y))
        throw std::runtime_error("Error: 0 is within the interval provided by i");
    return *this *= IntervalArithmetic{1.0f/i.x, 1.0f/i.y};
}

IntervalArithmetic IntervalArithmetic::pow(unsigned int pow) {
    if ((x <= 0) && (0 <= y))  {
        return {0, (float)std::max(std::pow(x, 2), std::pow(y, 2))};
    } else {
        return {(float)std::min(std::pow(x, 2), std::pow(y, 2)), (float)std::max(std::pow(x, 2), std::pow(y, 2))};
    }
}

std::ostream &operator<<(std::ostream &os, const IntervalArithmetic &arithmetic) {
    os << "[" << arithmetic.x << ", " << arithmetic.y << "]";
    return os;
}

IntervalArithmetic fprime(const IntervalArithmetic &x) {
    return x / (x - IntervalArithmetic{1, 1});
}

IntervalArithmetic fsecond(const IntervalArithmetic &x) {
    return IntervalArithmetic{1, 1} + IntervalArithmetic{1, 1} / (x - IntervalArithmetic{1, 1});
}

void examples_on_interval_arithmetic() {
    IntervalArithmetic ics{1,3};
    IntervalArithmetic six{6, 6};
    std::cout << (ics * (six - ics)) << std::endl; // [3, 15]

    std::cout << fprime({3.0, 3.1}) << std::endl;   // [1.42857, 1.55]
    std::cout << fsecond({3.0, 3.1}) << std::endl;  // [1.47619, 1.5]
}

