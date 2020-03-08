//
// Created by giacomo on 24/01/2020.
//

#include <cassert>
#include <iostream>
#include "ai/mlp/BackwardPropagationNetwork.h"

std::ostream &operator<<(std::ostream &os, const BackwardPropagationNetwork &network) {
    os << "layers: " << network.layers;
    return os;
}

void train_xor_bbn() {
    srand(1);
    //BackwardPropagationNetwork nn{2, {2,  1} };


    BackwardPropagationNetwork nn;
    nn.createLayer().addPerceptron({-0.394383, -0.197551}, -0.55397)
            .addPerceptron({-0.79844, 0.76823}, 0.628871);
    nn.createLayer().addPerceptron({0.513401, -0.916195}, 0.717297);
    nn.finalize();

    //std::cout << nn << std::endl;

    finite_function ff;
    ff.extendMap(0, {1, 1});
    ff.extendMap(1, {1, 0});
    ff.extendMap(1, {0, 1});
    ff.extendMap(0, {0, 0});
    nn.train(ff);

    std::cout << nn << std::endl;

    std::cout << nn.compute(ff.finite_function[0].input)[0] << std::endl;
    std::cout << nn.compute(ff.finite_function[1].input)[0] << std::endl;
    std::cout << nn.compute(ff.finite_function[2].input)[0] << std::endl;
    std::cout << nn.compute(ff.finite_function[3].input)[0] << std::endl;
}

BackwardPropagationNetwork::BackwardPropagationNetwork() {}

Layer &BackwardPropagationNetwork::createLayer() {
    layers.emplace_back();
    return layers.back();
}

BackwardPropagationNetwork::BackwardPropagationNetwork(size_t inputSize, const std::vector<size_t> &layersSize,
                                                       bool useRE) : inputSize(inputSize),
                                                                     layersSize(layersSize), useRE{useRE} {

    for (size_t i = 0; i<layersSize.size(); i++) {
        if (i == 0) {
            if (useRE)
                layers.emplace_back(inputSize, layersSize[0], re);
            else
                layers.emplace_back(inputSize, layersSize[0]);
        } else {
            if (useRE)
                layers.emplace_back(layersSize[i-1], layersSize[i], re);
            else
                layers.emplace_back(layersSize[i-1], layersSize[i]);
        }
    }

}

std::vector<double> BackwardPropagationNetwork::compute(std::vector<double> &input) {

    // Input for the first layer
    std::vector<double> currentInput = input;
    // Compute the intermediate output of each layer, and forward propagate that to the next layer
    for (Layer& layer : layers) {
        std::vector<double> intermediateOutput = layer.compute(currentInput);
        currentInput = intermediateOutput;
    }
    return currentInput; // Return the output provided by the last layer
}

void BackwardPropagationNetwork::reset() { for (Layer& l : layers) l.reset(); }

void BackwardPropagationNetwork::reset(std::default_random_engine &re) { for (Layer& l : layers) l.reset(re); }

double BackwardPropagationNetwork::calculateQuadraticError(std::vector<double> &output) {
    assert (!layersSize.empty());
    return layers[layersSize.size() - 1].calculateQuadraticError(output);
}

void BackwardPropagationNetwork::calculateDerivatives(std::vector<double> expectedOutput) {
    static size_t N = layersSize.size();
    for (int i = N-1; i >= 0; i--) {
        expectedOutput = layers[i].calculateDerivative(expectedOutput, (i == N - 1));
    }
}

void BackwardPropagationNetwork::gradientUpdate() { for (Layer& p : layers) p.updateGradient(); }

void BackwardPropagationNetwork::updateWeight(double learningRate, double momentum) {
    for (Layer& p : layers) p.updateWeight(learningRate, momentum);
}

double BackwardPropagationNetwork::train(struct finite_function &f, size_t iterationNumber, const double learningRate,
                                         const double momentum) {
    size_t epoch = 0;
	double error;
    while (epoch < iterationNumber)
    {

        error = 0.0;
        size_t N = f.finite_function.size();

        // For all the elements in the finite function f:
        for (int i = 0; i < N; i++) {
            // Train the network over the current input
            std::vector<double> result = compute(f.finite_function[i].input);
            // Calculate the error just for the heuristic's sake
            error += std::pow(calculateQuadraticError(f.finite_function[i].output), 2);

            // Perform the backpropagation algorithm over this input
            calculateDerivatives(f.finite_function[i].output);

            // Update gradient and weight of each neuron in the network independently
            gradientUpdate();
            updateWeight(learningRate, momentum);
        }
        error = std::sqrt(error);
        std::cout << error << "--" << epoch << std::endl;
        epoch = epoch + 1;

        //If the neural network is not converging after 4000 epochs, it might be possible that this network configuration does not provide a good local minimum. Therefore, it might be useful to reset the weights of the network and restart the training
        if (epoch > 4000 && error > 0.5) {
            if (useRE) reset(re); else reset(re);
            epoch = 0;
        }
    }

    // Error associated to the final configuration
	return error;
}

void BackwardPropagationNetwork::finalize() {
    for (const Layer& l : layers)
        layersSize.emplace_back(l.perceptrons.size());
}
