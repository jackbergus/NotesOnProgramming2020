//
// Created by giacomo on 31/01/2020.
//

#ifndef TUTORIALS_RULESFROMFREQUENTITEMSET_H
#define TUTORIALS_RULESFROMFREQUENTITEMSET_H

#include <dm/GeneralToSpecificHeuristic.h>
#include <dm/Rule.h>
#include <dm/DataMiningMetric.h>

/**
 * Implementation of the General-to-Speicific Heuristic algorithm for the Rule mining algorithm
 */
class RulesFromFrequentItemset : public GeneralToSpecificHeuristic<Rule> {
    DataMiningMetrics& scorer;

protected:
    /**
     * The quality assessment is going to return only the most plausible elements
     *
     * @param hypothesis
     * @return
     */
    bool quality_assessment(const std::pair<double, Rule> &hypothesis) const override;

    /**
     *
     * @param hypothesis
     * @return
     */
    std::vector<std::pair<double, Rule>> specialize_hypothesis(const Rule &hypothesis) const override;

    /**
     * Pruning all the elements from the queue that do not have a positive lift (i.e., a lift greater than 1)
     * @param queue
     */
    void prune(PriorityQueue &queue) override;

public:
    RulesFromFrequentItemset(DataMiningMetrics &scorer);

    std::set<Rule> generate_hypotheses(const Pattern<std::string>& pattern);
};


#endif //TUTORIALS_RULESFROMFREQUENTITEMSET_H
