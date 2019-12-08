#include <iostream>
#include "big_o_notation/fibonacci_no_master_theorem.h"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#define MAX_ITERATIONS      (30)
#define SAMPLES             (10)


int main() {

    fibonacci_no_master_theorem factory;;

    // Binet's formula
    double average = 0;
    for (unsigned int i = 0; i<MAX_ITERATIONS; i++) {
        auto before = Clock::now();
        for (unsigned int j = 0; j<MAX_ITERATIONS; j++) {
            factory.get(fibonacci_no_master_theorem::BINET_FORMULA, i);
        }
        auto t = (Clock::now() - before)/(MAX_ITERATIONS);
        std::cout << t.count();
        average += t.count();
        if (i == MAX_ITERATIONS-1)
            std::cout << std::endl;
        else
            std::cout << ",";
    }
    average /= MAX_ITERATIONS;

    // Using recursion with memoization just for the first number, so to observe the worst case scenario
    for (unsigned int i = 0; i<MAX_ITERATIONS; i++) {
        auto before = Clock::now();
        for (unsigned int j = 0; j<MAX_ITERATIONS; j++) {
            factory.get(fibonacci_no_master_theorem::RECURSIVE_WITH_MEMOIZATION, i);
            factory.clearMemoization();
        }
        auto t = (Clock::now() - before)/(MAX_ITERATIONS);
        std::cout << std::round(t.count()/average);
        if (i == MAX_ITERATIONS-1)
            std::cout << std::endl;
        else
            std::cout << ",";
    }

    // Using recursion with memoization, starting from the first element, so to observe the best case scenario
    for (unsigned int i = 0; i<MAX_ITERATIONS; i++) {
        auto before = Clock::now();
        for (unsigned int j = 0; j<MAX_ITERATIONS; j++) {
            factory.get(fibonacci_no_master_theorem::RECURSIVE_WITH_MEMOIZATION, i);
        }
        auto t = (Clock::now() - before)/(MAX_ITERATIONS);
        std::cout << std::round(t.count()/average);
        if (i == MAX_ITERATIONS-1)
            std::cout << std::endl;
        else
            std::cout << ",";
    }

    // Recursive function with no memoization (purely recursive)
    for (unsigned int i = 0; i<MAX_ITERATIONS; i++) {
        auto before = Clock::now();
        for (unsigned int j = 0; j<MAX_ITERATIONS; j++) {
            factory.get(fibonacci_no_master_theorem::RECURSIVE, i);
        }
        auto t = (Clock::now() - before)/(MAX_ITERATIONS);
        std::cout << std::round(t.count()/average);
        if (i == MAX_ITERATIONS-1)
            std::cout << std::endl;
        else
            std::cout << ",";
    }


    return 0;
}