//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_PERCEPTRON_H
#define TUTORIALS_PERCEPTRON_H

#include <vector>
#include "weight.h"
#include "../utils/printers.h"

std::vector<double> operator*(const std::vector<double>& lhs, double rhs);
std::vector<double> operator*(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double>& operator+=(std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator*(const std::vector<weight> &lhs, double rhs);


/**
 * The perceptron is the atomic component of a neural network
 */
struct Perceptron {
    /**
     * Weights associated to the input signal. These weights are the one used also for the momentum equation, and therefore
     * They remember also the weight in the previous computation step
     */
    std::vector<weight> weights;

    /**
     * Weight associated to the theta threshold: this acts as a dummy input having always a signal of 1, but which weight
     * can be changed.
     */
    weight theta;

    /**
     * Gradient value associated to the dummy/theta imput
     */
    double theta_gradient;

    /**
     * Copy of the input values: these will be useful once we have to compute the gradient.
     */
    std::vector<double> input_values;

    /**
     * Gradients associated to the current neuron
     */
    std::vector<double> input_gradients;
    size_t neuronIdInLayer;

    /**
     * Value returned by the sigmoid function.
     */
    double out;
    /**
     * Weighted sum provided to the sigmoid function
     */
    double net;
    /**
     * If this neuron belongs to the outer layer, this will store the difference between the output value and the
     * expected one. This will be then used for the derivative calculation
     */
    double error;

    double derivative;

    /**
     * Utility printing function
     * @param os
     * @param perceptron
     * @return
     */
    friend std::ostream &operator<<(std::ostream &os, const Perceptron &perceptron);

    /**
     * Using the default random initialization of the weights;
     */
    Perceptron();

    /**
     * Randomly setting up a neuron using the default random initialization of the weights, but using a specific neuron
     * id, that will be the neuron id within the layer
     *
     * @param neuronIdInLayer
     */
    Perceptron(size_t neuronIdInLayer, size_t N);

    /**
     * Importing the weights and the theta from a previously-saved configuration
     *
     * @param weights
     * @param theta
     */
    Perceptron(size_t neuronIdInLayer, std::vector<weight> weights, const weight &theta);

    Perceptron(size_t neuronIdInLayer, std::vector<double> weights, const double theta);

    /**
     *
     * @param id        Number associated to the current neuron
     * @param N         Number of the input size, that is the number of the neurons in the previous layer or the length of the input
     * @param re        Random number generator
     */
    Perceptron(size_t id, size_t N, std::default_random_engine& re);

    /**
     * Computing the function of the neuron
     *
     * @param input Input coming from the outside, that will be also saved in the inside.
     *
     * @return computed value
     */
    double compute(std::vector<double>& input);

    /**
     * Calculates the discrepancy between the computed value and the expected value. This function shall be called only
     * over output layer networks
     *
     * @param expectedValue
     * @return  Difference
     */
    double calculateError(double expectedValue);

    /**
     * Gradient calculation for the other layer networks
     *
     * @param deltasFromForward
     * @return
     */
    std::vector<double> calculateDerivative(double deltasFromForward);

    /**
     * Gradient calculation for the output layer neurons
     *
     * @param expectedValue
     * @return
     */
    std::vector<double> calculateDerivativeFromExpected(double expectedValue);

    void updateGradient();

    /**
     * Updates the weights of the current perceptron using the previous computation of the gradient.
     *
     * @param learningRate
     * @param momentum
     */
    void updateWeight(double learningRate = 1.414213562, double momentum = 0.25);

    void reset();

    void reset(std::default_random_engine &re);
};



#endif //TUTORIALS_PERCEPTRON_H
