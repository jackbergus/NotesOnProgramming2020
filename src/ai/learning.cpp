
//
// Created by ngb113 on 23/01/2020.
//
#include <ostream>
#include <unordered_map>
#ifdef _WIN64
#include <limits>			// in Windows, the only way to get numeric_limits
#include <algorithm>		// in Windows, the only way to get min
#endif

#include <dlib/dnn.h>
#include <dlib/mlp.h>

#include <ai/mlp/BackwardPropagationNetwork.h>
#include "ai/mlp/SinglePerceptron.h"
#include "ai/SupportVectorMachines.h"
#include "utils/numeric/permutation.h"
#include "ai/metrics/class_prediction_distance.h"
#include <ai/datasets/DLibSplits.h>


void mlp_train(const DLib_Splits& splits, const unsigned numberClasses, const size_t input_size) {/*
    // Even better, we could change the former source code to ensure that all the training and testing datasets for the
    // 8 classes will have the same size.
*/
    // The mlp takes column vectors as input and gives column vectors as output.  The dlib::matrix
    // object is used to represent the column vectors. So the first thing we do here is declare
    // a convenient typedef for the matrix object we will be using.
    const size_t iterationNumber = 100;

    double candidateScore = std::numeric_limits<double>::max();
    dlib::mlp::kernel_1a_c candidateNet(1, 1);    // dummy net: at the end, will store the best trained one

    std::cout << "Training is starting..." << std::endl;
    size_t maxFirstLayer = std::min(((unsigned long)(numberClasses+1L) * 5L), (unsigned long)30L);
    std::cout << "maxFirstLayer: " << maxFirstLayer << std::endl;
    for (int firstLayer =  maxFirstLayer; firstLayer >= 0; firstLayer--) {
        size_t maxSecondLayer = std::max(std::floor(((numberClasses+1.0) + firstLayer)/2.0), (numberClasses+1.0)*2.0);
        std::cout << "-- maxSecondLayer: " << maxSecondLayer << std::endl;
        for (size_t secondLayer = 0; secondLayer < maxSecondLayer; secondLayer++) {

            // Create a multi-layer perceptron network.   This network has 2 nodes on the input layer
            // (which means it takes column vectors of length 2 as input) and 5 nodes in the first
            // hidden layer.  Note that the other 4 variables in the mlp's constructor are left at
            // their default values.
            dlib::mlp::kernel_1a_c net(input_size, firstLayer, secondLayer, numberClasses+1);
            size_t epoch = 0;

            double distance;
            while (epoch < iterationNumber) {
                // Now let's put some data into our sample and train on it.  We do this
                // by looping over 41*41 points and labeling them according to their
                // distance from the origin.
                distance = 0.0;
                size_t N = std::min(splits.training_input.size(), splits.training_label_vector.size());

                for (size_t i = 0; i<N; i++) {
                    net.train(splits.training_input[i], splits.training_label_vector[i]);
                }

                for (size_t i = 0; i<N; i++) {
                    double ithDistance = 0.0;
                    auto prediction = net(splits.training_input[i]);
                    const auto& expected = splits.training_label_vector[i];
                    /*for (size_t j = 0; j<prediction.size(); j++) {
                        ithDistance += std::pow(prediction(j) - expected(j), 2);
                    }
                    ithDistance /= ((double)prediction.size());
                    ithDistance = std::sqrt(ithDistance);*/
                    /// Sanity check: checking that both the vector representation and the training label have the expected representation
                    ///assert(class_prediction_distance(splits.training_label_vector[i], splits.training_labels[i]) == 0.0);
                    ithDistance = class_prediction_distance(prediction, splits.training_labels[i]);

                    distance += ithDistance;
                }

                distance /= ((double)N);
                epoch = epoch + 1;
                //std::cout << distance << "~" << epoch << std::endl;
                // In this case, doing no reset at all.
            }

            std::cout << "firstLayer: " << firstLayer << " secondLayer: " << secondLayer << " score: " << distance << std::endl;
            if (distance >= 0.67803) {
                /// Heuristic: from the previous training, it seems that this is the maximum error that can be achieved, and that if I reach this value, then no further configuration will work
                std::cout<< "break!" << std::endl;
                break;
            }
            if (distance < candidateScore) {
                std::cout << "==> New candidate! " <<  std::endl;
                candidateScore = distance;
                candidateNet.swap(net); // Putting in candidateNet the best candidate: net is going to be discarded after quitting this loop anyway
            }
        }
    }


    // Now we have trained our mlp.  Let's see how well it did.
    // Note that if you run this program multiple times you will get different results. This
    // is because the mlp network is randomly initialized.

    // each of these statements prints out the output of the network given a particular sample.

    size_t N = std::min(splits.testing_label_vector.size(), splits.testing_input.size());
    double distance = 0.0;
    for (size_t i = 0; i<N; i++) {
        double ithDistance = 0.0;
        auto prediction = candidateNet(splits.testing_input[i]);
        const auto& expected = splits.testing_label_vector[i];
        ithDistance =  class_prediction_distance(prediction, splits.testing_labels[i]);
        // normalize the distance
        ithDistance = ithDistance / (ithDistance+1.0);
        distance += ithDistance;
    }
    // distance normalization
    distance = (distance) / ((double)N);
    // invert the distance, so to obtain the precision
    double precision = 1.0 - distance;

    std::cout << "Model precision over the testing data: " << precision << std::endl;

}

#include <dlib/svm_threaded.h> // contains one_vs_one_trainer
#include <ai/datasets/Starcraft9Dimensions.h>
#include <ai/datasets/training_testing_sets.h>

void multi_svm_train(const DLib_Splits& splits, const unsigned numberClasses, const size_t input_size) {

    // The main object in this example program is the one_vs_one_trainer.  It is essentially
    // a container class for regular binary classifier trainer objects.  In particular, it
    // uses the any_trainer object to store any kind of trainer object that implements a
    // .train(samples,labels) function which returns some kind of learned decision function.
    // It uses these binary classifiers to construct a voting multiclass classifier.  If
    // there are N classes then it trains N*(N-1)/2 binary classifiers, one for each pair of
    // labels, which then vote on the label of a sample.
    //
    // In this example program we will work with a one_vs_one_trainer object which stores any
    // kind of trainer that uses our sample_type samples.
    typedef dlib::one_vs_one_trainer<dlib::any_trainer<dlib::matrix<double>>> ovo_trainer;

    // Finally, make the one_vs_one_trainer.
    ovo_trainer trainer;

    // Create the binary trainer
    typedef dlib::radial_basis_kernel<dlib::matrix<double>> rbf_kernel;
    dlib::krr_trainer<rbf_kernel> rbf_trainer;

    // Now tell the one_vs_one_trainer that, by default, it should use the rbf_trainer
    // to solve the individual binary classification subproblems.
    trainer.set_trainer(rbf_trainer);


    std::cout << "cross validation: \n" << cross_validate_multiclass_trainer(trainer, splits.training_input, splits.training_labels, 5) << std::endl;

    // Next, if you wanted to obtain the decision rule learned by a one_vs_one_trainer you
    // would store it into a one_vs_one_decision_function.
    dlib::one_vs_one_decision_function<ovo_trainer> df = trainer.train(splits.training_input, splits.training_labels);

    size_t N = std::min(splits.testing_label_vector.size(), splits.testing_input.size());
    double distance = 0.0;
    for (size_t i = 0; i<N; i++) {
        double ithDistance = 0.0;
        double prediction = df(splits.testing_input[i]);
        const auto& expected = splits.testing_label_vector[i];
        ithDistance = std::abs(prediction - (double)splits.testing_labels[i]);
        // normalize the distance
        ithDistance = ithDistance / (ithDistance+1.0);
        distance += ithDistance;
    }
    // distance normalization
    distance = (distance) / ((double)N);
    // invert the distance, so to obtain the precision
    double precision = 1.0 - distance;

    std::cout << "Model precision over the testing data: " << precision << std::endl;
}

#include <unordered_map>
#include <ai/datasets/StarcraftAllDimensions.h>
#include <ai/metrics/entropy_metric.h>
#include <ai/rtree/decision_tree.h>
#include <ai/metrics/gini_metric.h>

template <typename Metric> void multi_rtree_train() {
    DLib_Splits splits;
    const std::pair<const size_t, const size_t> &info = generateSplit<dataset_full_dimensions>("data/starcraft.csv", splits,
                                                                                               false, 0.7);

    // Training the model one class against the other
    using CLS = std::vector<decision_tree<Metric>>;
    CLS classifiers;
    for (double classe = 1.0; classe <= 8.0; classe++) {
        std::cout << classe << std::endl;
        struct decision_tree<Metric> predict_class(splits, classe);
        predict_class.expand(5);
        classifiers.emplace_back(predict_class);
        std::cout << std::endl << std::endl;
    }

    size_t N = std::min(splits.testing_label_vector.size(), splits.testing_input.size());
    double distance = 0.0;
    for (size_t i = 0; i<N; i++) {
        double ithDistance = 0.0;
        std::pair<double, std::set<double>> result = decision_tree<Metric>::template classify<Metric>(classifiers, splits.testing_input[i]);
        ithDistance = class_prediction_distance(splits.testing_labels[i], result.second);
        // normalize the distance
        ithDistance = ithDistance / (ithDistance+1.0);
        distance += ithDistance;
    }
    // distance normalization
    distance = (distance) / ((double)N);
    // invert the distance, so to obtain the precision
    double precision = 1.0 - distance;

    std::cout << "Model precision over the testing data: " << precision << std::endl;

}

int main(void) {
    // A) Learning the And, Or and Xor function
    // Neural network simple examples
    training_or_perceptron();
    training_and_perceptron();
    training_xor_perceptron1_fail();
    training_xor_perceptron2_fail();
    train_xor_bbn();

    // SVN simple examples
    train_binary_svm(bool_and);
    train_binary_svm(bool_or);
    train_binary_svm(bool_xor);

    // B) Real world games dataset: StarCraft 2 replay files
    // https://developer.ibm.com/patterns/analyze-starcraft-ii-replays-with-jupyter-notebooks/
    // For these exaples, we're going to use the BBN from DLib

    // Loading the Starcraft Replay dataset, and selecting the relevant dimensions using the different class:
    // either dataset_9_dimensions or dataset_full_dimensions can be used
	// WARNING! Please check that the "data" folder is located in the Project Directory, or that the executable
	// is run in the same folder where the dataset is
    DLib_Splits splits;
    const std::pair<const size_t, const size_t> &info = generateSplit<dataset_full_dimensions>("data/starcraft.csv", splits,
                                                                                               true, 0.7);

    // Using DLib's implementation of the mutliclass svm classifier
    std::cout << "Multiple SVM Training" << std::endl;
    multi_svm_train(splits, info.first, info.second);
    std::cout << std::endl << std::endl;

    // Using DLib's implementation of the multilayer perceptron network
    std::cout << "MLP Training" << std::endl;
    mlp_train(splits, info.first, info.second);
    std::cout << std::endl << std::endl;

    std::cout << "Multi RTree Training (Need to reload a non-normalized dataset)" << std::endl;
    multi_rtree_train<entropy_metric>();
    std::cout << std::endl << std::endl;
}
