//
// Created by giacomo on 27/01/2020.
//

#ifndef TUTORIALS_SINGLEPERCEPTRON_H
#define TUTORIALS_SINGLEPERCEPTRON_H


#define DERIVATIVE(net)         (exp(net) / pow((1 + exp(net)), 2))
#define TRANSFER_DERIVATIVE(o)          ((o) * (1.0 - (o)))
#define ERROR(e, o)                     (((e) - (o)) * TRANSFER_DERIVATIVE(o))

#include <vector>
#include <functional>
#include <cmath>
#include <iostream>

#ifdef _WIN64
#include <limits>			// in Windows, the only way to get numeric_limits
#include <algorithm>		// in Windows, the only way to get min
#endif

/*
 * This model implements the easiest definition of a single neuron/perceptron, as in the one from the old ADALINE model
 */
struct single_perceptron {
    /**
     * Associating as many double values as many elements are there
     */
    std::vector<double> input_weights;

    /**
     * Activation threshold, under which no signal is provided
     */
    double threshold;

    /**
     * Delta error computed at the current step
     */
    double delta;

    /**
     * Previous value of net
     */
    double net;

    /**
     * Activation function
     */
    std::function<double(double)> sigma = [](double net) { return 1.0 / (1.0 + std::exp(-net)); };

    /**
     * Output stored from the previous computation of the calculate run
     */
    double previous_run_output;

    size_t estimate_input_size();

    double calculate(std::vector<double>& inputs);

    single_perceptron& addWeight(double x);

    single_perceptron& setWeight(size_t i, double x);

    single_perceptron& setThreshold(double th);


    /*
    * Delta rule for the back propagation as established by the backpropagation algorithm in 1986 by rumelhart, Hinton and Williams
    */
    void deltaRuleSinglePerceptron(std::vector<double>& inputs, double expectedOutput, double learningRate = 0.01);

    void updateWeights(std::vector<double>& inputs, double learningRate = 0.01, double momentum = 0.25);

    /*
    * Delta rule for the back propagation as established by the backpropagation algorithm in 1986 by rumelhart, Hinton and Williams

    * @returns the weighted delta to be back-propagated to all the other remaining layers
    */
    /*std::vector<double> deltaRule_withDelta(std::vector<double>& inputs, double delta, double learningRate = 0.01) {

    }*/



    /**
    * Training function using the delta rule over a given number of steps.
    */
    void train_perceptron(std::vector<std::vector<double>> trainingSet, std::vector<double> predictionValues, double learningRate = 0.01, size_t numberOfIterations = 100) {
        size_t M = std::min(trainingSet.size(), predictionValues.size());
        while (numberOfIterations > 0) {
            for (size_t i = 0; i < M; i++) {
                deltaRuleSinglePerceptron(trainingSet[i], predictionValues[i], learningRate);
            }
            numberOfIterations--;
        }
    }
};

void training_or_perceptron();
void training_and_perceptron();
void training_xor_perceptron1_fail();
void training_xor_perceptron2_fail();

#endif //TUTORIALS_SINGLEPERCEPTRON_H
