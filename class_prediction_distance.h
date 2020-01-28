//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_CLASS_PREDICTION_DISTANCE_H
#define TUTORIALS_CLASS_PREDICTION_DISTANCE_H

#include <dlib/matrix.h>
#include <set>

double class_prediction_distance(const dlib::matrix<double>& m, const unsigned long expected);

#endif //TUTORIALS_CLASS_PREDICTION_DISTANCE_H
