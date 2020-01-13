#include <iostream>
#include "big_o_notation/fibonacci_no_master_theorem.h"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

void doBenchmark(fibonacci_no_master_theorem &factory, double& average,  fibonacci_no_master_theorem::algorithm x, bool memoization, bool doBaseline);

#define MAX_ITERATIONS      (30)
#define SAMPLES             (50)

int main() {
    fibonacci_no_master_theorem factory;;

    // Binet's formula
    double average = 0;
    doBenchmark(factory, average, fibonacci_no_master_theorem::BINET_FORMULA, false, true);

    // Using recursion with memoization just for the first number, so to observe the worst case scenario
    doBenchmark(factory, average, fibonacci_no_master_theorem::RECURSIVE_WITH_MEMOIZATION, true, false);

    // Using recursion with memoization, starting from the first element, so to observe the best case scenario
    doBenchmark(factory, average, fibonacci_no_master_theorem::RECURSIVE_WITH_MEMOIZATION, false, false);

    // Recursive function with no memoization (purely recursive)
    doBenchmark(factory, average, fibonacci_no_master_theorem::RECURSIVE, false, false);

    return 0;
}

void doBenchmark(fibonacci_no_master_theorem &factory, double& average, fibonacci_no_master_theorem::algorithm x, bool clearMemoization, bool doBaseline) {
    for (unsigned int i = 1; i < MAX_ITERATIONS; i++) {
        auto before = Clock::now();
        for (unsigned int j = 0; j<SAMPLES; j++) {
            factory.get(x, i);
            if (clearMemoization) factory.clearMemoization();
        }
        auto t = (Clock::now() - before)/(SAMPLES);
        if (doBaseline) {
            std::cout << t.count(); average += t.count();
        } else
            std::cout << round(t.count()/average);
        if (i == MAX_ITERATIONS-1)
            std::cout << std::endl;
        else
            std::cout << ",";
    }
    if (doBaseline)
        average /= MAX_ITERATIONS;
}
