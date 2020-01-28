//
// Created by giacomo on 27/01/2020.
//

#ifndef TUTORIALS_SUPPORTVECTORMACHINES_H
#define TUTORIALS_SUPPORTVECTORMACHINES_H

#include <iostream>

void train_binary_svm(bool(*binary_function)(int, int));

// Custom function used for training
bool bool_and(int l, int r);
bool bool_or(int l, int r);
bool bool_xor(int l, int r);


#endif //TUTORIALS_SUPPORTVECTORMACHINES_H
