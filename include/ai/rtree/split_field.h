//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_SPLIT_FIELD_H
#define TUTORIALS_SPLIT_FIELD_H

#include <unordered_map>
#include <ostream>

using metric_row = std::unordered_map<std::string, double>; // same as typedef

struct split_field {
    std::string field_name;
    double      entropy_gain = 0.0;
    double      splitting_point = -std::numeric_limits<double>::max();
    double      precision = 0.0;
    bool        is_pos_leaf = false;
    bool        is_neg_leaf = false;
    bool        is_not_discriminative = false;
    bool        is_equal              = false;

    split_field(const struct split_field& x ) = default;
    split_field& operator=(const struct split_field& x) = default;
    split_field() = default;
    friend std::ostream &operator<<(std::ostream &os, const split_field &field);
};


#endif //TUTORIALS_SPLIT_FIELD_H
