//
// Created by giacomo on 06/12/2019.
//

#include <utils/y_combinator.h>
#include "big_o_notation/fibonacci_no_master_theorem.h"

fibonacci_no_master_theorem::fibonacci_no_master_theorem() {

    // Recursive lambda function
    // This implementation has exponential computational complexity
    {
        auto fibonacci_recursive = make_y_combinator([](auto&& fibonacci_recursive, unsigned long x) -> unsigned long {
            if(x <= 1)
                return x;
            else
                return fibonacci_recursive(x-1) + fibonacci_recursive(x-2);
        });
        map.insert(std::make_pair(RECURSIVE, fibonacci_recursive));
    }

    // Memoization lambda function (Memoization function acting like a cache)
    // In the worst case scenario, this implementation has linear computational complexity
    {
        auto  fibonacci_memoization= make_y_combinator([this](auto&& fibonacci_memoization, unsigned long x)->unsigned long {
            if (x<=1)
                return x;
            else {
                auto f = memoization.find(x);
                if (f != memoization.end())
                    return f->second;
                else {
                    unsigned long value = fibonacci_memoization(x-1) + fibonacci_memoization(x-2);
                    memoization.emplace(x, value);
                    return value;
                }

            }
        });
        map.insert(std::make_pair(RECURSIVE_WITH_MEMOIZATION, fibonacci_memoization));
    }

    // Binet Formula
    // If we approximate the mathematical operations as being O(1), then this implementation is the most efficient
    map.insert(std::make_pair(BINET_FORMULA, [](unsigned long n)->unsigned long {
        static const double phi = (1 + sqrt(5))*0.5;
        double fib = (pow(phi,n) - pow(1-phi,n))/sqrt(5);
        return round(fib);
    }));
}

void fibonacci_no_master_theorem::clearMemoization() {
    memoization.clear();
}
