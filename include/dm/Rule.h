//
// Created by giacomo on 31/01/2020.
//

#ifndef TUTORIALS_RULE_H
#define TUTORIALS_RULE_H

#include <vector>
#include <string>
#include <fptree.hpp>
#include <ostream>

/**
 * Subset relation among two vectors
 *
 * @tparam T    Types of the values associated to the vector
 * @param A     Left vector, the supposed superset
 * @param B     Right vector, the supposed subset
 * @return      Whether B is a subset of or equal to A
 */
template <typename T> bool IsSupsetOf(std::vector<T> A, std::vector<T> B) {
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    return std::includes(A.begin(), A.end(), B.begin(), B.end());
}

/**
 * Represents a simple mined rule
 */
struct Rule {
    std::vector<std::string> head; ///< Represents the head of the rule
    std::vector<std::string> tail; ///< Represents the tail of the rule

    Rule() = default;
    Rule(const Rule& rule) = default;
    Rule& operator=(const Rule& rule) = default;

    /**
     * Creating a rule from a vector having the head filled and the tail empty
     * @param itemset
     */
    Rule(std::vector<std::string>& itemset);

    /**
     * Creating a rule from a pattern having the head filled and the tail empty
     * @param itemset
     */
    Rule(const Pattern<std::string>& itemset);

/**
 * Printing some rules in output
 * @param os
 * @param rule
 * @return
 */
    friend std::ostream &operator<<(std::ostream &os, const Rule &rule);

    /**
     * Comparison operator: using the formal definition of precedence!
     */
    bool operator<(const Rule &rhs) const;

    bool operator>(const Rule &rhs) const;

    bool operator<=(const Rule &rhs) const;

    bool operator>=(const Rule &rhs) const;
};



#endif //TUTORIALS_RULE_H
