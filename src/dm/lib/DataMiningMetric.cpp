//
// Created by giacomo on 31/01/2020.
//

#include <dm/Rule.h>
#include "dm/DataMiningMetric.h"

DataMiningMetrics::DataMiningMetrics(const std::set<Pattern<std::string>> &S) {
    for (auto x : S) { // For each pattern
        std::vector<std::string> v{}; // Store it as a vector
        v.reserve(x.first.size());
        for (auto it = x.first.begin(); it != x.first.end(); ) {
            v.push_back(std::move(x.first.extract(it++).value()));
        }
        f[v] = x.second; // Associate the support to the rule represented as a vector
        sumAll += x.second; // Increment the number of the (frequent) transactions (Suggestion: replace it with the actual size of |T|)
    }
}

size_t DataMiningMetrics::support(const std::vector<std::string> &i) const {
    size_t sum = 0;
    for (auto it = f.begin(); it != f.end(); it++) {
        if (IsSupsetOf(it->first, i)) {
            sum += it->second;
        }
    }
    return sum;
}

double DataMiningMetrics::support(const Rule &r) const {
    std::vector<std::string> unione;
    for (const std::string& x: r.head) unione.emplace_back(x);
    for (const std::string& x: r.tail) unione.emplace_back(x);
    std::sort(unione.begin(), unione.end());
    unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
    return ((double)support(unione)) / sumAll;
}

double DataMiningMetrics::lift(const Rule &r) const {
    std::vector<std::string> unione;
    for (const std::string& x: r.head) unione.emplace_back(x);
    for (const std::string& x: r.tail) unione.emplace_back(x);
    std::sort(unione.begin(), unione.end());
    unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
    double pup = ((double)support(unione))/sumAll;
    double ppred = ((double)support(r.head) / sumAll);
    double psucc = ((double)support(r.tail) / sumAll);
    double final = (pup)/(ppred*psucc);
    return final;
}
