//
// Created by giacomo on 07/12/2019.
//

#ifndef TUTORIALS_VPTREE_NODE_H
#define TUTORIALS_VPTREE_NODE_H

#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

/**
 * From Knuth's the art of computer programming
 */
template <typename T> bool approximatelyEqual(T a, T b)
{
    return std::abs(a - b) <= ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * std::numeric_limits<T>::epsilon());
}

/**
 * From Knuth's the art of computer programming
 */
template  <typename T>  bool definitelyLessThan(T a, T b)
{
    return (b - a) > ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * std::numeric_limits<T>::epsilon());
}

/**
 * A kernel function provides the similarity between two elements
 * @tparam T
 */
template <typename T> struct DistanceFunction {
    DistanceFunction() = default;
    virtual ~DistanceFunction() = default;
    virtual double distance(const T& lhs, const T& rhs) {
        return (&lhs == &rhs);
    }
};


template <typename Point> struct vp_node {

    unsigned int id; ///<@ id associated to the current node (i.e., insertion order)
    Point pt;        ///<@ value to be compared
    double radius;   ///<@ radius that, in the standard definition, provides the boundary between the left and right nodes
    unsigned int leftChild = std::numeric_limits<unsigned int>::max(); ///<@ setting the leftChild to nullptr
    unsigned int rightChild = std::numeric_limits<unsigned int>::max(); ///<@ setting the rightChild to nullptr
    bool isLeaf = false; ///<@ nevertheless, each node is not necessairly a leaf

    vp_node(unsigned int id, Point pt, double radius) : id(id), pt(pt), radius(radius) {}
};

template <typename Point, typename DistanceFunction> struct vp_tree {
    std::vector<vp_node<Point>> tree; ///<@ representing the tree as a vector
    DistanceFunction ker;             ///<@ instance of the distance function provided by the template parameter
    std::mt19937 rng;                 ///<@ random number generator

    // Please note: this is not a thread-safe implementation
    double tau;                         ///<@ current distance decision value at the iteration
    std::vector<size_t> found;          ///<@ current found elements for the given lookup query

    /**
     * Initializing the element with a vector of elements
     * @param ls
     */
    vp_tree(const std::vector<Point>& ls) {
        tree.reserve(ls.size());
        size_t i = 0;
        for (auto x : ls)
            tree.emplace_back(i++, x, 0);
        recursive_restruct_tree(0, ls.size()-1);
    }

    /**
     * Initializing the tree with some static values
     * @param ls
     */
    vp_tree(std::initializer_list<Point> ls) {
        tree.reserve(ls.size());
        size_t i = 0;
        for (auto x : ls)
            tree.emplace_back(i++, x, 0);
        recursive_restruct_tree(0, ls.size());
    }

    /**
     * Looking for the elements near to id that are not exactely the same as him
     * @param id
     */
    void topk1Search(size_t id) {
        found.clear();
        tau    = std::numeric_limits<double>::max();
        lookUpNearsetTo(0, id);
    }

private:
    void lookUpNearsetTo(size_t root_id, size_t id) {
        //std::cout << root_id << std::endl;
        auto node = tree[id];
        auto root = tree[root_id];
        double rootRadius = tree[root_id].radius;
        double dist = ker.distance(tree[root_id].pt, node.pt);

        if ((root_id != id)) { // do not add the same object
            // If I find a better object, then discard all the previous ones, and set the current one
            if (definitelyLessThan(dist,tau)) {
                found.clear();
                found.emplace_back(root_id);
                tau = dist;
            } else
                // Otherwise, if they are very similar, then I could add this other one too
                if (approximatelyEqual(dist, tau)) {
                found.emplace_back(root_id);
                tau = std::min(dist, tau);
            }
        }

        // Continuing with the traversal depending on the distance from the root
        if (dist < rootRadius) {
            if (root.leftChild != std::numeric_limits<unsigned int>::max() && dist - tau <= rootRadius)
                lookUpNearsetTo(root.leftChild, id);

            if (root.rightChild != std::numeric_limits<unsigned int>::max() && dist + tau >= rootRadius)
                lookUpNearsetTo(root.rightChild, id);
        } else {
            if (root.rightChild != std::numeric_limits<unsigned int>::max() && dist + tau >= rootRadius)
                lookUpNearsetTo(root.rightChild, id);

            if (root.leftChild != std::numeric_limits<unsigned int>::max() && dist - tau <= rootRadius)
                lookUpNearsetTo(root.leftChild, id);
        }
    }

    void recursive_restruct_tree(size_t first, size_t last) {
        if (first >= last) {
            // If the elements overlaps, then I reached a leaf node
            tree[first].isLeaf = true;
            tree[first].leftChild = std::numeric_limits<unsigned int>::max();
            tree[first].rightChild = std::numeric_limits<unsigned int>::max();
        } else {
            if ((last - first) <= 1) {
                // If the elements differ by two, then I decide that one is the root, and the other is the child
                tree[first].radius = ker.distance(tree[first].pt, tree[last].pt);
                tree[first].leftChild = last;
                tree[first].rightChild = std::numeric_limits<unsigned int>::max();
            } else {
                // Picking the root randomly as the first element of the tree ~ O(1)
                std::uniform_int_distribution<int> uni(first, last - 1);
                int root = uni(rng);
                std::swap(tree[first], tree[root]);

                size_t median = (first + last) / 2; // TODO: other heuristic, separating the elements within a fixed radius from the object, and the ones out

                /*
                 * nth_element is a partial sorting algorithm that rearranges elements in [first, last) such that:
                 *
                 * - The element pointed at by median is changed to whatever element would occur in that position if [first, last) were sorted.
                 * - All of the elements before this new nth element are less than or equal to the elements after the new nth element.
                 */
                std::nth_element(
                        tree.begin() + first + 1,//first
                        tree.begin() + median,   //median
                        tree.begin() + last,    //last
                        [first, this] (const vp_node<Point>& i1, const vp_node<Point>& i2) {
                            return ker.distance(tree[first].pt, i1.pt) < ker.distance(tree[first].pt, i2.pt);
                        });

                // Setting the separating elements
                tree[first].radius = ker.distance(tree[first].pt, tree[median].pt);

                // Recursively splitting in half the elements within my radius and the ones out
                tree[first].leftChild = first+1;
                tree[first].rightChild = (first + last) / 2 + 1;
                recursive_restruct_tree(tree[first].leftChild, tree[first].rightChild-1);
                recursive_restruct_tree(tree[first].rightChild, last);

            }
        }
    }
};

#endif //TUTORIALS_VPTREE_NODE_H
