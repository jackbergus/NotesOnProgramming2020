//
// Created by giacomo on 31/01/2020.
//

#include "dm/Rule.h"

Rule::Rule(std::vector<std::string> &itemset) : head{itemset}, tail{} {}

Rule::Rule(const Pattern<std::string> &itemset) : head{itemset.first.size()} {
    // Copying the set's elements inside the predecessor
    std::copy(itemset.first.begin(), itemset.first.end(), head.begin());
}

std::ostream &operator<<(std::ostream &os, const Rule &rule) {
    os << "{";
    for (size_t i = 0, N = rule.head.size(); i < N; i++) {
        os << rule.head[i];
        if (i != (N-1)) os << ", ";
    }
    os << "} => {";
    for (size_t i = 0, N = rule.tail.size(); i < N; i++) {
        os << rule.tail[i];
        if (i != (N-1)) os << ", ";
    }
    os << "}";
    return os;
}

bool Rule::operator<(const Rule &rhs) const {
    return (IsSupsetOf(tail, rhs.tail) && IsSupsetOf(rhs.head, head));
}

bool Rule::operator>(const Rule &rhs) const {
    return rhs < *this;
}

bool Rule::operator<=(const Rule &rhs) const {
    return !(rhs < *this);
}

bool Rule::operator>=(const Rule &rhs) const {
    return !(*this < rhs);
}
