//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_ENTROPY_METRIC_H
#define TUTORIALS_ENTROPY_METRIC_H



struct entropy_metric {
    double node_iota(double freq_posLeft);
    double posterior(double sizeLeft, double sizeTotal, double posLeft, double posRight);
};


#endif //TUTORIALS_ENTROPY_METRIC_H
