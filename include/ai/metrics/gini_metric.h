//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_GINI_METRIC_H
#define TUTORIALS_GINI_METRIC_H



struct gini_metric {
    double metric_root(double pos, double neg) const;
    double subtree_part(double freq_posLeft);
    double posterior(double sizeLeft, double sizeTotal, double posLeft, double posRight);
};


#endif //TUTORIALS_GINI_METRIC_H
