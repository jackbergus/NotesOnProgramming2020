//
// Created by giacomo on 31/01/2020.
//

#include "dm/RulesFromFrequentItemset.h"

bool RulesFromFrequentItemset::quality_assessment(const std::pair<double, Rule> &hypothesis) const {
    return hypothesis.first > 1; // Getting only the plausible elements
}

std::vector<std::pair<double, Rule>> RulesFromFrequentItemset::specialize_hypothesis(const Rule &hypothesis) const {
    std::vector<std::pair<double, Rule>> result;
    for (const std::string& x : hypothesis.head) { // For each x appearing in the head of a hypothesis...
        Rule newRule;                              // ... generate a novel rule ...
        {
            // ... having as a tail the tail of hypothesis where x is added ...
            std::vector<std::string> succ{hypothesis.tail};
            succ.emplace_back(x);
            // (Please Note: the fastest approach of removing duplicates from a vector is to create a set, and then move it back to a vector)
            {
                std::set<std::string> setSucc( succ.begin(), succ.end() );
                newRule.tail.assign(setSucc.begin(), setSucc.end() );
            }
        }

        // ... and as a head all the elements belonging to the hypothesis's head minus x, that was currently added to the tail
        for (const std::string& y : hypothesis.head)
            if (x != y) newRule.head.emplace_back(y);

        // ... Associate a score (e.g., lift) to the newly generated rule
        result.emplace_back(scorer.lift(newRule), newRule);
    }
    // Return all the ranked specializations of the hypothesis
    return result;
}

void RulesFromFrequentItemset::prune(GeneralToSpecificHeuristic<Rule>::PriorityQueue &queue) {
    /*
     * Priority queues do not allow to directly remove the elements: therefore, I need to always remove the elements
     * from the queue, and then emplace the element in v if it meets the lift requirements
     */
    std::vector<std::pair<double, Rule>> v;
    while (!queue.empty()) {
        auto qt = queue.top();
        if ((scorer.lift(qt.second)) > 1) {
            v.emplace_back(qt);                 // Preserving the element
        }
        queue.pop();                            // Always removing the element from the queue
    }
    auto it = v.begin();                        // Iterating over the vector, while freeing the memory
    while (it != v.end()) {
        queue.push(*it);                        // Re-adding the preserved element back
        it = v.erase(it);                       // Clearing the memory from the vector.
    }
}

RulesFromFrequentItemset::RulesFromFrequentItemset(DataMiningMetrics &scorer) : scorer(scorer) {
}

std::unordered_set<Rule> RulesFromFrequentItemset::generate_hypotheses(const Pattern<std::string> &pattern) {
    Rule r{pattern};
    return GeneralToSpecificHeuristic::generate_hypotheses(r, scorer.lift(r));
}
