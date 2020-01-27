#if 1
//
// Created by ngb113 on 23/01/2020.
//
#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <ai/BackwardPropagationNetwork.h>

#define DERIVATIVE(net)         (exp(net) / pow((1 + exp(net)), 2))
#define TRANSFER_DERIVATIVE(o)          ((o) * (1.0 - (o)))
#define ERROR(e, o)                     (((e) - (o)) * TRANSFER_DERIVATIVE(o))

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

    size_t estimate_input_size() {
        return input_weights.size();
    }

    double calculate(std::vector<double>& inputs) {
        size_t N = std::min(input_weights.size(), inputs.size());

        net = 0.0;
        for (int i = 0; i < N; i++) {
            net += input_weights[i] * inputs[i];
        }


        return (previous_run_output = sigma((net+threshold)));
    }

    single_perceptron& addWeight(double x) {
        input_weights.emplace_back(x);
        return *this;
    }

    single_perceptron& setWeight(size_t i, double x) {
        input_weights[i] = x;
        return *this;
    }

    single_perceptron& setThreshold(double th) {
        threshold = th;
        return *this;
    }


    /*
    * Delta rule for the back propagation as established by the backpropagation algorithm in 1986 by rumelhart, Hinton and Williams
    */
    void deltaRuleSinglePerceptron(std::vector<double>& inputs, double expectedOutput, double learningRate = 0.01) {
        double predictionOutput = calculate(inputs);
        this->delta = predictionOutput - expectedOutput;
        this->delta = this->delta * (((delta)* expectedOutput * (1.0 - expectedOutput)));

        size_t N = std::min(input_weights.size(), inputs.size());
        for (int i = 0; i < N; i++) {
            double deltaW = learningRate * this->delta * inputs[i];
            input_weights[i] += deltaW;
        }
    }

    void updateWeights(std::vector<double>& inputs, double learningRate = 0.01, double momentum = 0.25) {
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


#if 0


/*
 In the old style model, a layer was just composed of different neurons.
 */
struct old_style_layer {
    std::vector<single_perceptron> neurons;

    size_t estimate_input_size() {
        size_t insize = 0;
        for (const single_perceptron& perceptron : neurons) {
            insize = std::max(insize, perceptron.input_weights.size());
        }
        return insize;
    }

    /*
    * The layer is composed by a given number of neurons
    */
    old_style_layer(size_t s) : neurons{ s }  {
    }

    // default constructor: no neurons are allocated.
    old_style_layer() {}

    /*
     * Returns the output for each possible neuron within the layer
     */
    std::vector<double> calculate(std::vector<double>& input) {
        std::vector<double> output;
        for (single_perceptron& layer : neurons) {
            output.emplace_back(layer.calculate(input));
        }
        return output;
    }

    /*std::vector<double> computeDeltas(std::vector<double>& deltas) {

        const size_t N =  neurons.size();

        std::vector<double> elements;
        elements.resize(estimate_input_size()); // All the elements are initialized with zeros

        for (int i = 0; i < N; i++) {
            elements = elements + neurons[i].computeDeltas(deltas[i]);
        }
        return elements;
    }

    */

    void updateWeights(std::vector<double>& inputs, double learningRage = 0.01) {
        const size_t N = std::min(inputs.size(), neurons.size());
        for (int i = 0; i < N; i++) {
            neurons[i].updateWeights(inputs, learningRage);
        }
    }
};

#include <random>
#include "NeuralNetwork.h"
#include "utils/FiniteFunction.h"

struct simple_neural_network {
    std::vector<old_style_layer>		deep_nn;
    std::default_random_engine re;
    size_t inputSize;
    std::vector< size_t> neuronsPerLayers;


    simple_neural_network(size_t inputSize, std::vector< size_t> neuronsPerLayers) : inputSize{inputSize}, neuronsPerLayers{neuronsPerLayers} {
        for (const size_t& x : neuronsPerLayers)
            deep_nn.emplace_back(x); // Creating each layer by setting how many neurons that layer should have

        // Now, initializing the network with some random weights
        reinit_network();
    }

    void reinit_network(bool firstInitialization = true) {
        std::uniform_real_distribution<double> unif(-1, 1);

        for (int i = 0; i < neuronsPerLayers.size(); i++) {
            if (i == 0) {
                // getting the number of the weights from the input size
                for (int k = 0; k < neuronsPerLayers[i]; k++) {
                    for (int j = 0; j < inputSize; j++) {
                        firstInitialization ? deep_nn[i].neurons[k].addWeight(unif(re)) : deep_nn[i].neurons[k].setWeight(j, unif(re));
                    }
                    deep_nn[i].neurons[k].setThreshold(unif(re));
                }
            } else {
                // getting the number of the weights from the former layer's size
                for (int k = 0; k < neuronsPerLayers[i]; k++) {
                    for (int j = 0; j < neuronsPerLayers[i - 1]; j++) {
                        firstInitialization ? deep_nn[i].neurons[k].addWeight(unif(re)) : deep_nn[i].neurons[k].setWeight(j, unif(re));
                    }
                    deep_nn[i].neurons[k].setThreshold(unif(re));
                }
            }
        }
    }

    /*
     * Chains the output of each layer, and propagates forward to the front
     */
    std::vector<double> calculate(std::vector<double>& input, std::vector<std::vector<double>>& inputInAllLayers) {

        // Input for the first layer
        std::vector<double> currentInput = input;
        inputInAllLayers.emplace_back(input);

        for (old_style_layer& layer : deep_nn) {
            std::vector<double> intermediateOutput = layer.calculate(currentInput);
            inputInAllLayers.emplace_back(intermediateOutput);
            currentInput = intermediateOutput;
        }
        return currentInput;
    }

    double backpropagate_error(std::vector<double>& inputs, std::vector<double> expectedOutput, double learningRate = 1.42) {
        std::vector<std::vector<double>> inputInAllLayers;

        // Compute the output provided by the network at this round
        // This computation will also store in each layer its intermediate result by construction
        std::vector<double> calculateOutput = calculate(inputs, inputInAllLayers);

        // Backpropagate the deltas for the weight update
        double MSE_ERROR = 0.0;
        for (int i = deep_nn.size()-1; i >= 0; i--) {
            if (i != deep_nn.size()-1) {
                for (single_perceptron& x : deep_nn[i+1].neurons) {
                    double totalError = 0.0;
                    size_t j = 0;
                    for (const single_perceptron& y : deep_nn[i + 1].neurons) {
                        double tmp = y.input_weights[j] * y.delta;
                        if (std::isnan(tmp)) {
                            std::cerr << totalError << "+=" << y.input_weights[j] << "*" << y.delta << std::endl;
                        }
                        totalError += tmp;
                    }
                    if (std::isnan(DERIVATIVE(x.net))) {
                        std::cerr << x.delta << "=" << totalError << "*" << DERIVATIVE(x.net) << std::endl;
                    }
                    x.delta = totalError * DERIVATIVE(x.net);
                    j++;
                }
            } else {
                size_t j = 0;
                for (single_perceptron& x : deep_nn[i].neurons) {
                    MSE_ERROR += pow(expectedOutput[j] - calculateOutput[j], 2);
                    if (std::isnan(DERIVATIVE(x.net))) {
                        std::cerr << x.delta << "=" << (expectedOutput[j] - calculateOutput[j])  << "*" << DERIVATIVE(x.net) << std::endl;
                    }
                    x.delta = (expectedOutput[j] - calculateOutput[j]) * DERIVATIVE(x.net);
                    j++;
                }
                MSE_ERROR /= (double)j;
                MSE_ERROR = sqrt(MSE_ERROR);
            }
        }

        // Backpropagate the weights
        for (int i = deep_nn.size() - 1; i >= 0; i--) {
            deep_nn[i].updateWeights(inputInAllLayers[i], learningRate);
        }

        return MSE_ERROR;
    }

    /*void backpropagate(std::vector<double>& inputs, std::vector<double> expectedOutput, double learningRate = 0.01) {
        std::vector<std::vector<double>> inputInAllLayers;

        // Compute the output provided by the network at this round
        // This computation will also store in each layer its intermediate result by construction
        std::vector<double> calculateOutput = calculate(inputs, inputInAllLayers);

        // Compute the delta for each output layer neuron
        std::vector<double> outputDeltas;
        size_t N = std::min(expectedOutput.size(), calculateOutput.size());
        for (size_t i = 0; i < N; i++) {
            outputDeltas.emplace_back(calculateOutput[i] - expectedOutput[i]);
        }

        // Backpropagate the deltas for the weight update
        for (int i = deep_nn.size()-1; i >= 0; i--) {
            outputDeltas = deep_nn[i].computeDeltas(outputDeltas);
        }

        // Backpropagate the weights
        for (int i = deep_nn.size() - 1; i >= 0; i--) {
            deep_nn[i].updateWeights(inputInAllLayers[i], learningRate);
        }
    }*/

    /**
    * Training function using the delta rule over a given number of steps.
    */
    void train(std::vector<std::vector<double>> trainingSet, std::vector<std::vector<double>> predictionValues, double learningRate = 0.01, size_t numberOfIterations = 20000) {
        size_t epoch = 0;
        size_t M = std::min(trainingSet.size(), predictionValues.size());
        while (epoch < numberOfIterations)
        {
            // For all the 4 elements of the training
            double RMSE_ERROR = 0.0;
            for (size_t i = 0; i < M; i++) {
                RMSE_ERROR += std::pow(backpropagate_error(trainingSet[i], predictionValues[i], learningRate), 2);
            }
            RMSE_ERROR /= (double)M;
            RMSE_ERROR = std::sqrt(RMSE_ERROR);
            epoch = epoch + 1;

            //Adding some motivation so if the neural network is not converging after 4000 epochs it will start over again until it converges
            if (epoch > 4000 && RMSE_ERROR > 0.5) {
                epoch = 0;
                reinit_network(false);
            }
        }


        for (int k = 0; k<numberOfIterations; k++) {

        }
    }
};

void training_xor_function() {
    simple_neural_network nn{ 2, {2,  1} };

    std::vector<std::vector<double>> binaryCombinations{ {0,0}, {0,1}, {1,0}, {1,1} };
    //std::vector<std::vector<double>> outputCalculate{ {0,0}, {0,1}, {1,0}, {1,1} };

    nn.train(binaryCombinations, { {0},{1},{1},{0} }, 1.414213562, 20000);

    for (std::vector<double>& x : binaryCombinations) {
        std::vector<std::vector<double>> inputInAllLayers;
        std::cout << nn.calculate(x, inputInAllLayers)[0]  << std::endl;
    }

    std::cout << std::endl << std::endl;
}

#endif

void training_or_function() {
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

void training_and_function() {
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

void training_xor_function1_fail() {
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

void training_xor_function2_fail() {
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



void testing_working();


int main(void) {
    //training_or_function();
    //training_and_function();
    //training_xor_function1_fail();
    //training_xor_function2_fail();

/*
 *
 * weight 0 = -0.394383
weight 2 = -0.197551
weight 4 = -0.55397

weight 1 = -0.79844
weight 3 = 0.76823
weight 5 = 0.628871

 weight 6 = 0.513401
 weight 7 = -0.916195
weight 8 = 0.717297
 */

#if 1
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

    std::cout << nn.compute(ff.finite_function[0].input)[0] << std::endl;
    std::cout << nn.compute(ff.finite_function[1].input)[0] << std::endl;
    std::cout << nn.compute(ff.finite_function[2].input)[0] << std::endl;
    std::cout << nn.compute(ff.finite_function[3].input)[0] << std::endl;
#else
    testing_working();
#endif
}
#else
#endif