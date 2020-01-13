//
// Created by giacomo on 06/12/2019.
//

#ifndef TUTORIALS_FIBONACCI_NO_MASTER_THEOREM_H
#define TUTORIALS_FIBONACCI_NO_MASTER_THEOREM_H

#include <cstdint>
#include <cmath>
#include <unordered_map>
#include <functional>

unsigned long binet_formula(unsigned long n);

/**
 * Factory method class for all the possible Fibonacci number's generation.
 *
 * This is an example of a function where you cannot use the Master Theorem for proving it. Therefore, you need to
 * prove it by induction.
 */
class fibonacci_no_master_theorem {
public:
    /**
     * Defining all the possible algorithms
     */
    enum algorithm {
        RECURSIVE,
        RECURSIVE_WITH_MEMOIZATION,
        BINET_FORMULA,
    };

    /**
     * Constructor initializing the map with the lambda functions
     */
    fibonacci_no_master_theorem();

    /**
     * Runs the fibonacci algorithm with the selected strategy and over the given year value
     * @param choice		Selected algorithm/strategy
     * @param x			Year of choice
     * @return			Return value of the Fibonacci function
     */
    unsigned long get(algorithm choice, unsigned long x) {
        auto f = map.find(choice);
        if (f == map.end()) return 0; else
            return f->second(x);
    }

    /**
     * @brief Clearing all the memoized values.
     * This function will make you aware on the positive effects of memoization while
     */
    void clearMemoization();


private:
    /**
     * Map where all the lambda functions are stored
     */
    std::unordered_map<algorithm, std::function<unsigned long(unsigned long)>> map;

    /**
     * Memoization map used for an algorithm
     */
    std::unordered_map<unsigned long, unsigned long> memoization;
};


#endif //TUTORIALS_FIBONACCI_NO_MASTER_THEOREM_H
