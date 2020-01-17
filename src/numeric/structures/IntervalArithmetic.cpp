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
        return {0, (float)std::max(std::pow(x, pow), std::pow(y, pow))};
    } else {
        return {(float)std::min(std::pow(x, pow), std::pow(y, pow)), (float)std::max(std::pow(x, pow), std::pow(y, pow))};
    }
}

std::ostream &operator<<(std::ostream &os, const IntervalArithmetic &arithmetic) {
    os << "[" << arithmetic.x << ", " << arithmetic.y << "]";
    return os;
}

template <typename T> T fprime(const T &x, const T& one) {
    return x / (x - one);
}

template <typename T> T fsecond(const T &x, const T& one) {
    return one + one / (x - one);
}

void examples_on_interval_arithmetic() {
    IntervalArithmetic ics{1,3};
    IntervalArithmetic six{6, 6};
    std::cout << (ics * (six - ics)) << std::endl; // [3, 15]

    std::cout << fprime<IntervalArithmetic>({2.9, 3.1}, {1.0,1.0}) << std::endl;   // [1.38095, 1.63158]
    std::cout << fprime(3.0, 1.0) << std::endl;

    std::cout << fsecond<IntervalArithmetic>({2.9, 3.1}, {1.0,1.0}) << std::endl;  // [1.47619, 1.52632]
    std::cout << fsecond(3.0, 1.0) << std::endl;
}

