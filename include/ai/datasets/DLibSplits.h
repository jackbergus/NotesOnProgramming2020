//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_DLIBSPLITS_H
#define TUTORIALS_DLIBSPLITS_H

#include <vector>
#include <dlib/matrix.h>

struct DLib_Splits {
    std::vector<dlib::matrix<double>> training_input, testing_input, training_label_vector, testing_label_vector;
    std::vector<double> training_labels, testing_labels;

};

#endif //TUTORIALS_DLIBSPLITS_H
