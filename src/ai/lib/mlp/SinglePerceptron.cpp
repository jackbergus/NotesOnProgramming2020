//
// Created by giacomo on 27/01/2020.
//

#include "ai/SinglePerceptron.h"

size_t single_perceptron::estimate_input_size() {
    return input_weights.size();
}

double single_perceptron::calculate(std::vector<double> &inputs) {
    size_t N = std::min(input_weights.size(), inputs.size());

    net = 0.0;
    for (int i = 0; i < N; i++) {
        net += input_weights[i] * inputs[i];
    }


    return (previous_run_output = sigma((net+threshold)));
}

single_perceptron &single_perceptron::addWeight(double x) {
    input_weights.emplace_back(x);
    return *this;
}

single_perceptron &single_perceptron::setWeight(size_t i, double x) {
    input_weights[i] = x;
    return *this;
}

single_perceptron &single_perceptron::setThreshold(double th) {
    threshold = th;
    return *this;
}

void
single_perceptron::deltaRuleSinglePerceptron(std::vector<double> &inputs, double expectedOutput, double learningRate) {
    double predictionOutput = calculate(inputs);
    this->delta = predictionOutput - expectedOutput;
    this->delta = this->delta * (((delta)* expectedOutput * (1.0 - expectedOutput)));

    size_t N = std::min(input_weights.size(), inputs.size());
    for (int i = 0; i < N; i++) {
        double deltaW = learningRate * this->delta * inputs[i];
        input_weights[i] += deltaW;
    }
}

void single_perceptron::updateWeights(std::vector<double> &inputs, double learningRate, double momentum) {
    double predictionOutput = calculate(inputs);
    size_t N = std::min(input_weights.size(), inputs.size());
    for (int i = 0; i < N; i++) {
        if (std::isnan(input_weights[i])) {
            std::cerr << input_weights[i] <<  std::endl;
        }
        double tmp = delta  * learningRate * sigma(inputs[i]) + momentum * sigma(input_weights[i]);
        if (std::isnan(tmp)) {
            std::cerr << input_weights[i] << " += " << delta << " * "<< learningRate << " * " << sigma(inputs[i]) << "+" << momentum <<"*"<< input_weights[i] << std::endl;
        }
        input_weights[i] += tmp;
        //input_weights[i] -= learningRate * delta * inputs[i];
    }
    if (std::isnan(threshold)) {
        std::cerr << threshold <<  std::endl;
    }
    threshold += delta * sigma(1.0) * learningRate + momentum * threshold;
    if (std::isnan(threshold)) {
        std::cerr << threshold << " += " << delta << " * "<< sigma(1.0) << "*" << learningRate << "+" << momentum <<"*"<< threshold << std::endl;
    }
}

void training_or_perceptron() {
    single_perceptron perceptron;
    perceptron.addWeight(0.23).addWeight(0.73).setThreshold(0.2);

    std::vector<std::vector<double>> binaryCombinations{ {0.01,0.01}, {0.01,0.99}, {0.99,0.01}, {0.99,0.99} };
    std::vector<std::vector<double>> outputCalculate{ {0,0}, {0,1}, {1,0}, {1,1} };

    std::cout << "Calculated values over the randomly assigned values" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;

    perceptron.train_perceptron(binaryCombinations, {0.00001, 1, 1, 1}, 2000, 10000);

    std::cout << "Output for the trained values for the OR function" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "Output for the actual binary values" << std::endl;
    for (std::vector<double>& x : outputCalculate) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void training_and_perceptron() {
    single_perceptron perceptron;
    perceptron.addWeight(0.0000006).addWeight(0.00000006).setThreshold(1.0);

    std::vector<std::vector<double>> binaryCombinations{ {0.001,0.001}, {0.001,0.999}, {0.999,0.001}, {0.999,0.999} };
    std::vector<std::vector<double>> outputCalculate{ {0,0}, {0,1}, {1,0}, {1,1} };

    std::cout << "Calculated values over the randomly assigned values" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;

    perceptron.train_perceptron(binaryCombinations, {0.00001, 0.00001, 0.00001, 1}, 2000, 10000);

    std::cout << "Output for the trained values for the AND function" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "Output for the actual binary values" << std::endl;
    for (std::vector<double>& x : outputCalculate) {
        std::cout << perceptron.calculate(x) << std::endl;
    }


    std::cout << std::endl << std::endl;
}

void training_xor_perceptron1_fail() {
    single_perceptron perceptron;
    perceptron.addWeight(0.0000006).addWeight(0.00000006).setThreshold(1.0);

    std::vector<std::vector<double>> binaryCombinations{ {0.001,0.001}, {0.001,0.999}, {0.999,0.001}, {0.999,0.999} };
    std::vector<std::vector<double>> outputCalculate{ {0,0}, {0,1}, {1,0}, {1,1} };

    std::cout << "Calculated values over the randomly assigned values" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;

    perceptron.train_perceptron(binaryCombinations, {0.00001, 1, 1, 0.00001}, 2000, 10000);

    std::cout << "Output for the trained values for the XOR function" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "Output for the actual binary values" << std::endl;
    for (std::vector<double>& x : outputCalculate) {
        std::cout << perceptron.calculate(x) << std::endl;
    }


    std::cout << std::endl << std::endl;
}

void training_xor_perceptron2_fail() {
    single_perceptron perceptron;
    perceptron.addWeight(0.23).addWeight(0.73).setThreshold(0.2);

    std::vector<std::vector<double>> binaryCombinations{ {0.01,0.01}, {0.01,0.99}, {0.99,0.01}, {0.99,0.99} };
    std::vector<std::vector<double>> outputCalculate{ {0,0}, {0,1}, {1,0}, {1,1} };

    std::cout << "Calculated values over the randomly assigned values" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;

    perceptron.train_perceptron(binaryCombinations, {0.00001, 1, 1, 0.00001}, 2000, 10000);

    std::cout << "Output for the trained values for the XOR function" << std::endl;
    for (std::vector<double>& x : binaryCombinations) {
        std::cout << perceptron.calculate(x) << std::endl;
    }
    std::cout << "Output for the actual binary values" << std::endl;
    for (std::vector<double>& x : outputCalculate) {
        std::cout << perceptron.calculate(x) << std::endl;
    }


    std::cout << std::endl << std::endl;
}
