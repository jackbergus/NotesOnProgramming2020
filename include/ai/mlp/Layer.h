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
     *
     * @param previousLayerSize         Number of perceptrons in the previous layer/input
     * @param N                         Number of perceptrons in the current layer
     * @param re                        Random number generator
     */
    Layer(size_t previousLayerSize, size_t N, std::default_random_engine& re);

    Layer(size_t previousLayerSize, size_t N) {
        for (size_t j = 0; j < N; j++) {
            perceptrons.emplace_back(j, previousLayerSize);
        }
    }

    Layer();
    Layer& addPerceptron(std::vector<double> weights, double theta);

    std::vector<double> compute(std::vector<double>& input);

    void reset();

    void reset(std::default_random_engine& re);

    double calculateQuadraticError(std::vector<double>& expectedOuput);

    /**
     * Gradient calculation for the output layer neurons
     *
     * @param expectedValue
     * @return
     */
    std::vector<double> calculateDerivative(std::vector<double>& expectedValue, bool FromExpectedValue);

    void updateGradient();

    void updateWeight(double learningRate = 1.414213562, double momentum = 0.25);
};


#endif //TUTORIALS_LAYER_H
