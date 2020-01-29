//
// Created by giacomo on 24/01/2020.
//

#include <cassert>
#include "ai/mlp/Layer.h"

std::ostream &operator<<(std::ostream &os, const Layer &layer) {
    os << "perceptrons: " << layer.perceptrons << " currentOutput: " << layer.currentOutput;
    return os;
}

Layer::Layer(size_t previousLayerSize, size_t N, std::default_random_engine &re) {
    for (size_t j = 0; j < N; j++) {
        perceptrons.emplace_back(j, previousLayerSize, re);
    }
}

Layer::Layer() {}

Layer &Layer::addPerceptron(std::vector<double> weights, double theta) {
    perceptrons.emplace_back(perceptrons.size(), weights, theta);
    return *this;
}

std::vector<double> Layer::compute(std::vector<double> &input) {
    std::vector<double> result;
    for (Perceptron& p : perceptrons)
        result.emplace_back(p.compute(input));
    currentOutput = result;
    return currentOutput;
}

void Layer::reset() {
    for (Perceptron& p : perceptrons) p.reset();
}

void Layer::reset(std::default_random_engine &re) {
    for (Perceptron& p: perceptrons) p.reset(re);
}

double Layer::calculateQuadraticError(std::vector<double> &expectedOuput) {
    assert(perceptrons.size() == expectedOuput.size());
    double compute = 0.0;
    size_t N = expectedOuput.size();
    for (size_t i = 0; i<N; i++) {
        compute += std::pow(perceptrons[i].out - expectedOuput[i], 2);
    }
    ///std::cout << "SingleError = " << std::sqrt(compute) << std::endl;
    return std::sqrt(compute);
}

std::vector<double> Layer::calculateDerivative(std::vector<double> &expectedValue, bool FromExpectedValue) {
    assert(expectedValue.size() == this->perceptrons.size());
    size_t N = expectedValue.size();

    std::vector<double> result;
    for (size_t i = 0; i<N; i++) {
        result +=  FromExpectedValue ?
                   perceptrons[i].calculateDerivativeFromExpected(expectedValue[i]) :
                   perceptrons[i].calculateDerivative(expectedValue[i]);
    }

    return result;
}

void Layer::updateGradient() { for (Perceptron& p : perceptrons) p.updateGradient(); }

void Layer::updateWeight(double learningRate, double momentum) {
    for (Perceptron& p : perceptrons) p.updateWeight(learningRate, momentum);
}
