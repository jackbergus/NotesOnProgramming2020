//
// Created by giacomo on 31/01/2020.
//

#ifndef TUTORIALS_GENERALTOSPECIFICHEURISTIC_H
#define TUTORIALS_GENERALTOSPECIFICHEURISTIC_H

#include <queue>
#include <vector>
#include <unordered_set>


/**
 * Implementation of a generic general-to-specific computation with an heuristic pruning all the elements that do not satisfy the lift value
 *
 * @tparam DataType       Data Type over which we want to perform the element generation
 */
template <typename DataType> class GeneralToSpecificHeuristic {

protected:

    typedef std::priority_queue<std::pair<double, DataType>, // The data type that we are comparing
            std::vector<std::pair<double,DataType>> // The container of the data structure>
    >
            PriorityQueue;

    PriorityQueue queue;


    /**
     * Quality assessment predicate Q(h, D), where D is always the whole dataset. In particular, this quality assessment
     * function shall be a monotonic constraint.
     *
     * @param hypothesis    Hypothesis that we want to assess
     * @return              A boolean value
     */
    virtual bool quality_assessment(const std::pair<double, DataType>& hypothesis) const = 0;

    /**
     * Given an hypothesis, it provides a set of scored specializations
     *
     * @param hypothesis
     * @return
     */
    virtual std::vector<std::pair<double, DataType>> specialize_hypothesis(const DataType& hypothesis) const = 0;

    /**
     * Default method providing a pruning from the default queue
     *
     * @param queue     The queue will be updated accordingly to the predicate's specification
     */
    virtual void prune(PriorityQueue& queue) = 0;


public:
    GeneralToSpecificHeuristic() : queue{} {}

    std::unordered_set<DataType> generate_hypotheses(const DataType& top, double score) {
        std::unordered_set<DataType> hypotheses;

        // C++'s PriorityQueue doesn't have a clear method: need to hardcode that
        queue = PriorityQueue{};
        // Insert the first element of the queue
        queue.emplace(score, top);

        while (!queue.empty()) {
            const auto& h = queue.top();    // Passing the element by reference
            if (quality_assessment(h)) {
                queue.pop();                // I'm done copying it: I can free the element
            } else {
                auto h_second = h.second;   // Copy a part of the value...
                queue.pop();                // ... then, I can free it
                for (const auto& x : specialize_hypothesis(h_second)) {
                    queue.emplace(x);       // Re-populating the queue with the weighted specializations
                }
            }
            prune(queue);
        }

        return hypotheses;
    }

};

#endif //TUTORIALS_GENERALTOSPECIFICHEURISTIC_H
