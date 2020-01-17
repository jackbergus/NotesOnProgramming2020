//
// Created by giacomo on 10/01/2020.
//

#include <iostream>
#include <utils/float.h>
#include <limits>
#include "numeric/FibonacciTotalError.h"

float iterative_pow(float base, int pow) {
    float intermediate = 1.0;
    if (pow <= 0)
        return 1.0;
    if (pow == 1)
        return base;
    for (int i = 1; i<=pow; i++)
        intermediate *= base;
    return intermediate;
}

unsigned long binet_formula_error(unsigned long n) {
    float fib = (iterative_pow(((1.0f + (float)sqrtf(5.0f))*0.5f), n) - iterative_pow(( 1.0f - ((1.0f + (float)sqrtf(5.0f))*0.5f)), n))/sqrtf(5);
    return roundf(fib);
}


void printing_float_representation() {
    declare_printable_float(one, 1.23);
    std::cout << one << std::endl;

    declare_printable_float(two, 0.000000000000123); // 1.23e-13
    std::cout << two << std::endl;

    declare_printable_float(three, 12300000000000000.0f); // 1.23e+16
    std::cout << three << std::endl;

    std::cout << "Min" << std::endl;
#ifndef WINDOWS
    declare_printable_float(minfloat, std::numeric_limits<float>::min());
#else 
	declare_printable_float(minfloat, FLT_MIN);
#endif 
    std::cout << minfloat << std::endl;

    std::cout << "Max" << std::endl;
#ifndef WINDOWS
    declare_printable_float(maxfloat, std::numeric_limits<float>::max());
#else 
	declare_printable_float(maxfloat, FLT_MAX);
#endif 
    std::cout << maxfloat << std::endl;

    std::cout << "Lowest" << std::endl;
    declare_printable_float(varepsilon, std::numeric_limits<float>::lowest());
    std::cout << varepsilon << std::endl;

    std::cout << "Round Error" << std::endl;
    declare_printable_float(err, std::numeric_limits<float>::round_error());
    std::cout << err << std::endl;

    std::cout << "+Inf" << std::endl;
    declare_printable_float(inf, std::numeric_limits<float>::infinity());
    std::cout << inf << std::endl;
}

void total_error_plot() {
    for (int i = 3; i<90; i++) {
        std::cout <<  "i=" << i << " --> " << ((double)std::abs((long long)binet_formula_error(i) - (long long)binet_formula(i)))/((double)binet_formula(i)) << std::endl;
    }
}
