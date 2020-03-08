//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_SPLIT_FIELD_H
#define TUTORIALS_SPLIT_FIELD_H

#include <unordered_map>
#include <ostream>

/**
 * Defining the row assoicated to the dataset: a row is just a map assoicating an attribute to a given value
 */
using metric_row = std::unordered_map<std::string, double>; // same as typedef

struct split_field {
    std::string field_name;                                             ///<@ Attribute name performing the separation
    bool        is_equal    = false;                                    ///<@ If set to true, then it is the equality predicate, otherwise it is <
    double      splitting_point = -std::numeric_limits<double>::max();  ///<@ Value associated to the predicate
    double      impurity_decrease = 0.0;                                ///<@ Entropy gain associated to the current field
    double      precision = 0.0;                                        ///<@ Ratio of the samples belonging to the class over the elements that do not belong to it
    bool        is_pos_leaf = false;                                    ///<@ Determining whether the current node contains all elements that are positive examples
    bool        is_neg_leaf = false;                                    ///<@ Determining whether the current node contains all elements that are negative examples
    bool        is_not_discriminative = false;                          ///<@ The decision determined here is said to be not discriminative if it will end up having one empty child and the other containing the same dataset of the root

    split_field(const struct split_field& x ) = default;
    split_field& operator=(const struct split_field& x) = default;
    split_field() = default;
    friend std::ostream &operator<<(std::ostream &os, const split_field &field);

    bool operator==(const split_field &rhs) const;
    bool operator!=(const split_field &rhs) const;
};


#endif //TUTORIALS_SPLIT_FIELD_H
