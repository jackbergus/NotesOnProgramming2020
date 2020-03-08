//
// Created by giacomo on 24/01/2020.
//

#ifndef TUTORIALS_BACKWARDPROPAGATIONNETWORK_H
#define TUTORIALS_BACKWARDPROPAGATIONNETWORK_H


#include <ai/mlp/Layer.h>
#include <utils/functional/FiniteFunction.h>
#include <iostream>

struct BackwardPropagationNetwork {
    size_t inputSize;
    std::vector<size_t> layersSize;
    std::vector<Layer>  layers;
    std::default_random_engine re;
    bool useRE;

    friend std::ostream &operator<<(std::ostream &os, const BackwardPropagationNetwork &network);

    /**
     * Default constructor of an empty network
     */
    BackwardPropagationNetwork();

    /**
     * Generates a new layer, that is returned by reference
     * @return
     */
    Layer& createLayer();

    /**
     * Generates a network given a network configuration
     * @param inputSize             Size of the input
     * @param layersSize            Number of neurons per hidden layer and output layer
     * @param useRE                 If set to true and if the neural network is not converging after 4000 epochs,
     *                              it will reset the neuron weights and start over again until it converges
     */
    BackwardPropagationNetwork(size_t inputSize, const std::vector<size_t> &layersSize, bool useRE = false);

    /**
     *
     * @param input                 Input value
     * @return
     */
    std::vector<double> compute(std::vector<double> &input);

    void reset();

    void reset(std::default_random_engine& re);

    /**
     * Calculates the loss function
     * @param output
     * @return
     */
    double calculateQuadraticError(std::vector<double>& output);

    /**
     * Make all the neurons in the network calculate their derivatives
     * @param expectedOutput        Starts the derivatives computation from the quadratic error computed over the expected output
     */
    void calculateDerivatives(std::vector<double> expectedOutput);

    /**
     * Updates the gradient of all the neurons in the netwokr
     */
    void gradientUpdate();

    /**
     * Uèdates the weight using the backpropagation of all the neurons in the network
     * @param learningRate
     * @param momentum
     */
    void updateWeight(double learningRate = 1.414213562, double momentum = 0.25);

    /**
     * Trains the network to learn a hidden function which map is known and provided via f
     *
     * @param f                     Map of the function
     * @param iterationNumber       Maximum number of iterations that we can wait to reach convergence
     * @param learningRate
     * @param momentum
     * @return                      Loss function associated to the final configuration of the network (at the end of the training phase)
     */
    double train(struct finite_function& f, const size_t iterationNumber = 20000, const double learningRate = 1.414213562, const double momentum = 0.25);

    /**
     * Finalizes the network creation if I've been using layerCreate to create one single layer.
     */
    void finalize();
};


void train_xor_bbn();


#endif //TUTORIALS_BACKWARDPROPAGATIONNETWORK_H
