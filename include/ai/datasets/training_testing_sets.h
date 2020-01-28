//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_TRAINING_TESTING_SETS_H
#define TUTORIALS_TRAINING_TESTING_SETS_H

#include <unordered_set>
#include <iostream>
#include <csv.h>
#include <utils/permutation.h>
#include "DLibSplits.h"
#include "StarcraftReplayDataset.h"

/**
 * Utility struct providing a splitting between training and test data set from a set of unique elements.
 *
 * @tparam T            Type of the elements that need to be sorted
 * @tparam Hash         Hashing function associated to the elements
 * @tparam Eq           Equality function associated to the elements
 */
template <typename T, typename  HashEq>
struct training_testing_sets {
    std::unordered_set<T, HashEq, HashEq> training, testing;

    training_testing_sets(std::unordered_set<T, HashEq, HashEq>& originalSet, const double percentage = 0.7) {
        double i = 0, N = originalSet.size();
        for (auto it = originalSet.begin(); it != originalSet.cend(); it++) {
            (((i)/N < percentage) ? training : testing).emplace(*it);
            i += 1;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const training_testing_sets &sets) {
        double Tr = sets.training.size();
        double Ts = sets.testing.size();
        os << "training: " << (Tr)/(Tr+Ts) << " testing: " << (Ts)/(Tr+Ts);
        return os;
    }
};



template <typename HashEq>
std::pair<const size_t, const size_t>
generateSplit(const std::string &datasetName, DLib_Splits &out, const double trainingSize = 0.7) {
    // Reading the StarCraft dataset
    io::CSVReader<21> starcraft{datasetName};
    HashEq dlibMover;
    starcraft.read_header(io::ignore_extra_column, "GameID", "LeagueIndex", "Age", "HoursPerWeek", "TotalHours", "APM", "SelectByHotkeys", "AssignToHotkeys", "UniqueHotkeys", "MinimapAttacks", "MinimapRightClicks", "NumberOfPACs", "GapBetweenPACs", "ActionLatency", "ActionsInPAC", "TotalMapExplored", "WorkersMade", "UniqueUnitsMade", "ComplexUnitsMade", "ComplexAbilityUsed", "MaxTimeStamp");

    // This final map will contain both the training and the testing dataset
    // We will split the dataset into training and testing for each of the classes
    std::unordered_map<unsigned short, training_testing_sets<StarcraftReplayDataset, HashEq>> splitting_map;
    {
        // Reading all the datasets: using the custom hashing and equality functions, so that we automatically project the data as we wish
        std::cout << "Reading the CSV file" << std::endl;
        std::unordered_map<unsigned short, std::unordered_set<StarcraftReplayDataset, HashEq, HashEq>> map;
        bool continueReading = true;
        do {
            auto x = StarcraftReplayDataset::read(starcraft);
            if (x) {                                                // If it was possible to read the information from the file
                map[x->league_index].emplace(x.value());       // Insert it inside the map
            } else {
                continueReading = false;                            // Otherwise, fail and stop reading
            }
        } while (continueReading);

        // Defining the split size for the training/testing dataset. Inserting those after removing the duplicates.
        std::cout << "Splitting everything in human-readable training and testing sets" << std::endl;
        for (auto it = map.begin(); it != map.cend(); it++) {
            // Splitting the previously loaded set of data into the training and the testing dataset
            splitting_map.insert(std::make_pair(it->first, training_testing_sets<StarcraftReplayDataset, HashEq>{it->second, trainingSize}));
        }
    }

    // Preparing the loaded data in the format that DLib expects.
    // Please note that this multiple-step implementation is just to be debuggable and to be sure what we're doing,
    // but everything can be done more efficiently in one single step.
    std::cout << "Copying the information for a DLib-compatible representation" << std::endl;
    for (auto it = splitting_map.begin(); it != splitting_map.cend(); it++) {
        const training_testing_sets<StarcraftReplayDataset, HashEq>& splits = it->second;

        for (const StarcraftReplayDataset& trainingDatum : splits.training) {
            dlibMover.fit_sample(out.training_input, out.training_labels, trainingDatum);
            dlibMover.fit_output(out.training_label_vector, trainingDatum, splitting_map.size());
        }

        for (const StarcraftReplayDataset& testingDatum : splits.testing) {
            dlibMover.fit_sample(out.testing_input, out.testing_labels, testingDatum);
            dlibMover.fit_output(out.testing_label_vector, testingDatum, splitting_map.size());
        }
        std::cout << it->first << "~" << it->second << std::endl;
    }

    // Shuffling the data
    std::cout << "Performing a random data permutation" << std::endl;
    std::vector<int> permutation = generateRandomPermutation(out.training_input.size());
    out.training_input = permute(out.training_input, permutation);
    out.training_labels = permute(out.training_labels, permutation);
    out.training_label_vector = permute(out.training_label_vector, permutation);

    // <number of classes, number of dimensions == mlp input>
    std::cout << "Dataset ordering done" << std::endl;
    return {splitting_map.size(), dlibMover.dimensions};

#if 0

    using net_type = dlib::loss_multiclass_log<
            dlib::fc<19,
    dlib::cont<1,19,1,2,2,
           dlib::relu<dlib::con<4,19,1,2,2,
            dlib::input<dlib::matrix<double>>>>>>>;


    // So with that out of the way, we can make a network instance.
    net_type net;
    // And then train it using the data.  The code below uses mini-batch stochastic
    // gradient descent with an initial learning rate of 0.01 to accomplish this.
    dlib::dnn_trainer<net_type> trainer(net);
    trainer.set_learning_rate(0.99);
    trainer.set_min_learning_rate(0.00001);
    //trainer.set_mini_batch_size(10);
    trainer.be_verbose();
    // Since DNN training can take a long time, we can ask the trainer to save its state to
    // a file named "mnist_sync" every 20 seconds.  This way, if we kill this program and
    // start it again it will begin where it left off rather than restarting the training
    // from scratch.  This is because, when the program restarts, this call to
    // set_synchronization_file() will automatically reload the settings from mnist_sync if
    // the file exists.
    trainer.set_synchronization_file("mnist_sync", std::chrono::seconds(20));
    // Finally, this line begins training.  By default, it runs SGD with our specified
    // learning rate until the loss stops decreasing.  Then it reduces the learning rate by
    // a factor of 10 and continues running until the loss stops decreasing again.  It will
    // keep doing this until the learning rate has dropped below the min learning rate
    // defined above or the maximum number of epochs as been executed (defaulted to 10000).
    trainer.train(out.training_input, out.training_labels);

    // At this point our net object should have learned how to classify MNIST images.  But
    // before we try it out let's save it to disk.  Note that, since the trainer has been
    // running images through the network, net will have a bunch of state in it related to
    // the last batch of images it processed (e.g. outputs from each layer).  Since we
    // don't care about saving that kind of stuff to disk we can tell the network to forget
    // about that kind of transient data so that our file will be smaller.  We do this by
    // "cleaning" the network before saving it.
    net.clean();
    dlib::serialize("mnist_network.dat") << net;
    // Now if we later wanted to recall the network from disk we can simply say:
    // deserialize("mnist_network.dat") >> net;


    // Now let's run the training images through the network.  This statement runs all the
    // images through it and asks the loss layer to convert the network's raw output into
    // labels.  In our case, these labels are the numbers between 0 and 9.
    std::vector<unsigned long> predicted_labels = net(out.training_input);
    int num_right = 0;
    int num_wrong = 0;
    // And then let's see if it classified them correctly.
    for (size_t i = 0; i < out.training_input.size(); ++i)
    {
        if (predicted_labels[i] == out.training_labels[i])
            ++num_right;
        else
            ++num_wrong;

    }
    std::cout << "training num_right: " << num_right << std::endl;
    std::cout << "training num_wrong: " << num_wrong << std::endl;
    std::cout << "training precision:  " << num_right/(double)(num_right+num_wrong) << std::endl;

    // Let's also see if the network can correctly classify the testing images.  Since
    // MNIST is an easy dataset, we should see at least 99% accuracy.
    predicted_labels = net(out.testing_input);
    num_right = 0;
    num_wrong = 0;
    for (size_t i = 0; i < out.testing_input.size(); ++i)
    {
        if (predicted_labels[i] == out.testing_labels[i])
            ++num_right;
        else
            ++num_wrong;

    }
    std::cout << "testing num_right: " << num_right << std::endl;
    std::cout << "testing num_wrong: " << num_wrong << std::endl;
    std::cout << "testing accuracy:  " << num_right/(double)(num_right+num_wrong) << std::endl;


    // Finally, you can also save network parameters to XML files if you want to do
    // something with the network in another tool.  For example, you could use dlib's
    // tools/convert_dlib_nets_to_caffe to convert the network to a caffe model.
    net_to_xml(net, "lenet.xml");

#endif
}

#endif //TUTORIALS_TRAINING_TESTING_SETS_H
