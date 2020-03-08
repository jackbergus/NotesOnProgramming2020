//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_LAYER_H
#define TUTORIALS_LAYER_H


#include <ai/mlp/Perceptron.h>

/**
 * A layer is just an array of neurons
 */
struct Layer {
    std::vector<Perceptron> perceptrons;
    std::vector<double> currentOutput;

    friend std::ostream &operator<<(std::ostream &os, const Layer &layer);

    /**
     * Creating N neurons in the layer with some C++11-like randomly generated weights (via a random number generator)
     * @param previousLayerSize         Number of perceptrons in the previous layer/input
     * @param N                         Number of perceptrons in the current layer
     * @param re                        Random number generator
     */
    Layer(size_t previousLayerSize, size_t N, std::default_random_engine& re);

    /**
     * Creating N neurons in the layer with some C-like randomly generated weights
     * @param previousLayerSize
     * @param N
     */
    Layer(size_t previousLayerSize, size_t N) {
        for (size_t j = 0; j < N; j++) {
            perceptrons.emplace_back(j, previousLayerSize);
        }
    }

    /**
     * Creating an empty layer
     */
    Layer();

    /**
     * Adding a perceptron to the layer
     * @param weights
     * @param theta
     * @return
     */
    Layer& addPerceptron(std::vector<double> weights, double theta);

    /**
     * Evaluates the regression function over the data
     * @param input         Input value
     * @return              Returned prediction
     */
    std::vector<double> compute(std::vector<double>& input);

    /**
     * Reset the weights of all the neurons in the network
     */
    void reset();

    /**
     * Resets the weights of all the neurons in the network using a random number generator
     * @param re
     */
    void reset(std::default_random_engine& re);

    /**
     * Computes the quadratic error over the output previously returned by the neuron in the output layer
     * @param expectedOuput     Value that was expected to be returned
     * @return                  Quadratic error value
     */
    double calculateQuadraticError(std::vector<double>& expectedOuput);

    /**
     * Gradient calculation for the neurons (either in the output layer or in the input layer)
     *
     * @param expectedValue
     * @param FromExpectedValue     If the value is set to true, then @expectedValue is the expected value from the output.
     *                              Otherwise, that vector will contain the delta values to be backpropagated within the network
     * @return                      The value to be backpropagated from the network
     */
    std::vector<double> calculateDerivative(std::vector<double>& expectedValue, bool FromExpectedValue);

    /**
     * Recursively updating the gradient for each neuron in the layer
     */
    void updateGradient();

    /**
     * Recursively updating the gradient for each neuron in the layer
     * @param learningRate
     * @param momentum
     */
    void updateWeight(double learningRate = 1.414213562, double momentum = 0.25);
};


#endif //TUTORIALS_LAYER_H
