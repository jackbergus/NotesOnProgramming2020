//
// Created by giacomo on 29/01/2020.
//

#include "ai/rtree/split_field.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const split_field &field) {
    os << "field  '" << field.field_name << "'";
    if (field.is_pos_leaf) {
        os << " provides a positive leaf ";
    } else if (field.is_neg_leaf) {
        os << " provides a negative leaf ";
    } else {
        os << " is the best predictor with entropy gain=" << field.impurity_decrease;
    }
    os << " with cutoff point=" << field.splitting_point;
    return os;
}

bool split_field::operator==(const split_field &rhs) const {
    return field_name == rhs.field_name &&
           splitting_point == rhs.splitting_point &&
           is_equal == rhs.is_equal;
}

bool split_field::operator!=(const split_field &rhs) const {
    return !(rhs == *this);
}
