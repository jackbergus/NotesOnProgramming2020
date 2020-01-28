//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_ENTROPYTREE_H
#define TUTORIALS_ENTROPYTREE_H

#include <cmath>
#include <vector>
#include <algorithm>

double entropy_function(double class1, double class2) {
    double sum = class1 + class2;
    double p1 = class1 / sum;
    double p2 = class2 / sum;
    return (-p1) * std::log2(p1) + (-p2) * std::log2(p2);
}


class EntropyTree {

    std::vector<EntropyTree> children;
    double getClass1Samples() const { return 0; }
    double getClass2Samples() const { return 0; }

    double nodeEntropy() const {
        return entropy_function(getClass1Samples(), getClass2Samples());
    }

    double aPosterioriEntropy() {
        double sum_of_elems = 0.0;
        for (const EntropyTree& subtree : children)
            sum_of_elems += subtree.nodeEntropy();
        return sum_of_elems / ((double)children.size());
    }

    double informationGain() {
        return nodeEntropy() - aPosterioriEntropy();
    }
};


#endif //TUTORIALS_ENTROPYTREE_H
