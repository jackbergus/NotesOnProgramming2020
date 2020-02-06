//
// Created by giacomo on 07/12/2019.
//


#include <big_o_notation/vptree_node.h>
#include <iostream>

/**
 * Example
 */
struct DoubleKernel : public DistanceFunction<double> {
    double distance(const double &a, const double &b) override {
        return  (std::abs(a - b));
    }
};

int main(void) {
    std::vector<double> vec{{1,2,3,4,5,6,7,8,9,10,11,12}};
    vp_tree<double, DoubleKernel> tree{vec};
    size_t lookup = 4;
    tree.topk1Search(lookup);
    for (auto x : tree.found) {
        std::cout << tree.tree[lookup].pt << " ~ " << tree.tree[x].pt << std::endl;
    }
}
