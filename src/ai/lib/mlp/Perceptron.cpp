//
// Created by giacomo on 24/01/2020.
//

#include <algorithm>
#include "ai/mlp/Perceptron.h"
#include "utils/functional/Functionals.h"

#define SIGMOID(x)          ((1.0) / (1.0+ std::exp(-(x))))

static inline double gradient(double input, double derivative) { return (input * derivative); }
static inline double gradient(double derivative)               { return (derivative); }

std::ostream &operator<<(std::ostream &os, const Perceptron &perceptron) {
    os << "weights: " << perceptron.weights << " theta: " << perceptron.theta << " theta_gradient: "
       << perceptron.theta_gradient << " input_values: " << perceptron.input_values << " input_gradients: "
       << perceptron.input_gradients << " neuronIdInLayer: " << perceptron.neuronIdInLayer << " out: "
       << perceptron.out << " net: " << perceptron.net << " error: " << perceptron.error << " derivative: "
       << perceptron.derivative;
    return os;
}

Perceptron::Perceptron() : Perceptron{0, 2} {
}

Perceptron::Perceptron(size_t neuronIdInLayer, size_t N) : Perceptron(neuronIdInLayer, {}, {}) {
    weights.resize(N);
    input_gradients.resize(N);
}

Perceptron::Perceptron(size_t neuronIdInLayer, std::vector<weight> weights, const weight &theta) : weights{std::move(weights)}, theta{theta}, input_values{} {
    out = 0.0;
    net = 0.0;
    error = 0.0;
    theta_gradient = 0.0;
    for (size_t i = 0; i<weights.size(); i++) input_gradients.emplace_back(0.0);
}

Perceptron::Perceptron(size_t id, size_t N, std::default_random_engine &re) : neuronIdInLayer{id}, theta{re} {
    for (size_t i = 0; i<N; i++) {
        weights.emplace_back(re);
        input_gradients.emplace_back(0.0);
    }
}

double Perceptron::compute(std::vector<double> &input) { //Checked
    size_t N = std::min(input.size(), weights.size());
    input_values = input;

    net = 0.0;
    for (size_t i = 0; i<N; i++) {
        net += input[i] * weights[i].current;
    }
    net += theta.current;

    return (out = SIGMOID(net));
}

Perceptron::Perceptron(size_t neuronIdInLayer, std::vector<double> weights, const double theta) : theta{theta}, input_values{} {
    out = 0.0;
    net = 0.0;
    error = 0.0;
    theta_gradient = 0.0;
    for (size_t i = 0; i<weights.size(); i++) {
        this->weights.emplace_back(weights[i]);
        input_gradients.emplace_back(0.0);
    }
}

double Perceptron::calculateError(double expectedValue) {
    error = (out - expectedValue);
    return (error);
}

std::vector<double> Perceptron::calculateDerivative(double deltasFromForward) {
    derivative = (exp(net) / pow((1 + exp(net)), 2)) * deltasFromForward;
    return weights * derivative;
}

std::vector<double> Perceptron::calculateDerivativeFromExpected(double expectedValue) {
    return calculateDerivative(-calculateError(expectedValue));
}

void Perceptron::updateGradient() {
    for (size_t i = 0; i<input_values.size(); i++) {
        input_gradients[i] = gradient(input_values[i], derivative);
    }
    theta_gradient = gradient(derivative);
}

void Perceptron::updateWeight(double learningRate, double momentum) {  //ERROR
    double deltaWeight;
    for (size_t i = 0; i<weights.size(); i++) {
        weight& x = weights[i]; // ref
        deltaWeight = (learningRate * input_gradients[i]) + (momentum * x.previous);
        x.previous = deltaWeight;
        x.current = x.current + deltaWeight;
    }
    deltaWeight = (learningRate * theta_gradient) + (momentum * theta.previous);
    theta.previous = deltaWeight;
    theta.current = theta.current + deltaWeight;
}

void Perceptron::reset() {
    for (weight& w : this->weights) w.reset();
    theta.reset();
}

void Perceptron::reset(std::default_random_engine &re) {
    for (weight& w : this->weights) w.reset(re);
    theta.reset(re);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::vector<double> operator*(const std::vector<double> &lhs, double rhs) {
    std::vector<double> bar;
    std::transform (lhs.begin(), lhs.end(), bar.begin(), [rhs](double x) {return x*rhs; });
    return bar;
}

std::vector<double> operator+(const std::vector<double> &lhs, const std::vector<double> &rhs) {
    std::vector<double> result;
    my_transform(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter(result), 0.0, [](double x, double y) {return x+y;});
    return result;
}

std::vector<double> operator*(const std::vector<double> &lhs, const std::vector<double> &rhs) {
    std::vector<double> result;
    my_transform(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter(result), 0.0, [](double x, double y) {return x*y;});
    return result;
}

std::vector<double> operator*(const std::vector<weight> &lhs, double rhs) {
    std::vector<double> bar;
    std::transform (lhs.begin(), lhs.end(), std::back_inserter(bar), [rhs](weight x) {return x.current * rhs; });
    return bar;
}

std::vector<double> &operator+=(std::vector<double> &lhs, const std::vector<double> &rhs) {
    size_t N = lhs.size(), M = rhs.size();
    for (size_t i = 0; i<std::max(N, M); i++) {
        if (i<N) {
            lhs[i] += rhs[i];
        } else {
            lhs.emplace_back(rhs[i]);
        }
    }
    return lhs;
}

