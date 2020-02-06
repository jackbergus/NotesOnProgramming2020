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

    BackwardPropagationNetwork();

    Layer& createLayer();

    BackwardPropagationNetwork(size_t inputSize, const std::vector<size_t> &layersSize, bool useRE = false);

    std::vector<double> compute(std::vector<double> &input/*, std::vector<std::vector<double>>& inputInAllLayers*/);

    void reset();

    void reset(std::default_random_engine& re);

    double calculateQuadraticError(std::vector<double>& output);

    void calculateDerivatives(std::vector<double> expectedOutput);

    void gradientUpdate();

    void updateWeight(double learningRate = 1.414213562, double momentum = 0.25);

    double train(struct finite_function& f, const size_t iterationNumber = 20000, const double learningRate = 1.414213562, const double momentum = 0.25);

    void finalize();
};


void train_xor_bbn();


#endif //TUTORIALS_BACKWARDPROPAGATIONNETWORK_H
