//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_PRINTERS_H
#define TUTORIALS_PRINTERS_H

template <typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T> x) {
    os << "{";
    for (size_t i = 0; i<x.size(); i++) {
        os << x[i];
        if (i == x.size()-1)
            os << "}";
        else
            os << ", ";
    }
    return os;
}

#endif //TUTORIALS_PRINTERS_H
