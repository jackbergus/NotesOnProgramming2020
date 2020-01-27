//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_FUNCTIONALS_H
#define TUTORIALS_FUNCTIONALS_H

template <typename T1, typename T2, typename T3, typename F, typename T>
void my_transform(T1 beg1, T2 end1, T1 beg2, T2 end2, T3 inserter, T defaultValue, F func) {
    while (!((beg1 == end1) && (beg2 == end2))) {
        T left = (beg1 == end1) ? defaultValue : *beg1;
        T right = (beg2 == end2) ? defaultValue : *beg2;
        *inserter = func(left, right);
        if (beg1 != end1) beg1++;
        if (beg2 != end2) beg2++;
    }
}

#endif //TUTORIALS_FUNCTIONALS_H
