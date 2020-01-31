//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_DLIBSPLITS_H
#define TUTORIALS_DLIBSPLITS_H

#include <vector>
#include <dlib/matrix.h>

struct DLib_Splits {
    size_t dimension;                       ///< How many dimensions are provided in this numeric data
    std::vector<std::string> colnames;      ///< The properties' names associated to each dimension

    /*
     * The classes/labels are here represented as doubles, but the actual class value is discrete.
     */
    std::vector<double> training_labels, testing_labels;

    /*
     * Representing the input as columns of `dimension` dimensions
     */
    std::vector<dlib::matrix<double>> training_input, testing_input,

    /*
     * Each of the n labels is now represented as an (n+1)-dimensional vector, where the first dimension is never used.
     * In particular, the i-th label will be represented by an unit vector having the (i+1)-th component set to 1.
     * By doing so, we can interpret the vector returned by a classifier as follows: the element will belong to the i-th
     * class if the (i+1)-th component of the vector will be a
     */
    training_label_vector, testing_label_vector;


};

#endif //TUTORIALS_DLIBSPLITS_H
