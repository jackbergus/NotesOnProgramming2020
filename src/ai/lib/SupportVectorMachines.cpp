//
// Created by giacomo on 27/01/2020.
//

#include "ai/SupportVectorMachines.h"
#include <dlib/svm.h>

bool bool_and(int l, int r) { return (l && r); }
bool bool_or(int l, int r) { return (l || r); }
bool bool_xor(int l, int r) { return ((!l != !r)); }

void train_binary_svm(bool (*binary_function)(int, int)) {
    // The svm functions use column vectors to contain a lot of the data on which they
    // operate. So the first thing we do here is declare a convenient typedef.

    // This typedef declares a matrix with 2 rows and 1 column.  It will be the object that
    // contains each of our 2 dimensional samples.   (Note that if you wanted more than 2
    // features in this vector you can simply change the 2 to something else.  Or if you
    // don't know how many features you want until runtime then you can put a 0 here and
    // use the matrix.set_size() member function)
    typedef dlib::matrix<double, 2, 1> sample_type;

    // This is a typedef for the type of kernel we are going to use in this example.  In
    // this case I have selected the radial basis kernel that can operate on our 2D
    // sample_type objects
    // https://en.wikipedia.org/wiki/Radial_basis_function_kernel
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;

    // Now we make objects to contain our samples and their respective labels.
    std::vector<sample_type> samples;
    std::vector<double> labels;

    // Now let's put some data into our samples and labels objects.  We do this by looping
    // over a bunch of points and labeling them according to their distance from the
    // origin.
    for (int r = 0; r <= 1; ++r)
    {
        for (int c = 0; c <= 1; ++c)
        {
            sample_type samp;
            samp(0) = r;
            samp(1) = c;
            samples.push_back(samp);
            labels.push_back(binary_function(r, c) ? 1 : -1);
        }
    }




    // Here we normalize all the samples by subtracting their mean and dividing by their
    // standard deviation.  This is generally a good idea since it often heads off
    // numerical stability problems and also prevents one large feature from smothering
    // others.  Doing this doesn't matter much in this example so I'm just doing this here
    // so you can see an easy way to accomplish this with the library.
    dlib::vector_normalizer<sample_type> normalizer;
    // let the normalizer learn the mean and standard deviation of the samples
    normalizer.train(samples);
    // now normalize each sample
    for (unsigned long i = 0; i < samples.size(); ++i)
        samples[i] = normalizer(samples[i]);


    // Now that we have some data we want to train on it.  However, there are two
    // parameters to the training.  These are the nu and gamma parameters.  Our choice for
    // these parameters will influence how good the resulting decision function is.  To
    // test how good a particular choice of these parameters is we can use the
    // cross_validate_trainer() function to perform n-fold cross validation on our training
    // data.  However, there is a problem with the way we have sampled our distribution
    // above.  The problem is that there is a definite ordering to the samples.  That is,
    // the first half of the samples look like they are from a different distribution than
    // the second half.  This would screw up the cross validation process but we can fix it
    // by randomizing the order of the samples with the following function call.
    randomize_samples(samples, labels);


    // The nu parameter has a maximum value that is dependent on the ratio of the +1 to -1
    // labels in the training data.  This function finds that value.
    const double max_nu = dlib::maximum_nu(labels);

    // here we make an instance of the svm_nu_trainer object that uses our kernel type.
    dlib::svm_nu_trainer<kernel_type> trainer;

    // Now we loop over some different nu and gamma values to see how good they are.
    // model_selection_ex.cpp from the dlib library provides a different approach to pick the best gamma and nu values.
    std::cout << "doing cross validation" << std::endl;
    double candidateGamma = 0.0, candidateNu = 0.0;
    double scoresforCandidates = std::numeric_limits<double>::max();

    for (double gamma = 0.00001; gamma <= 1; gamma *= 5)
    {
        for (double nu = 0.00001; nu < max_nu; nu *= 5)
        {
            // Here we are making an instance of the normalized_function object.  This object
            // provides a convenient way to store the vector normalization information along with
            // the decision function we are going to learn.

            // Set the kernel gamma and nu values from the iteration loop
            typedef dlib::decision_function<kernel_type> dec_funct_type;
            typedef dlib::normalized_function<dec_funct_type> funct_type;
            funct_type learned_function;
            trainer.set_kernel(kernel_type(gamma));
            trainer.set_nu(nu);

            // Extract the learned function
            learned_function.normalizer = normalizer;  // save normalization information
            learned_function.function = trainer.train(samples, labels); // perform the actual SVM training and save the results

            // Evaluating the mean square error between the predictions and the actual value
            double distance = 0.0;
            for (size_t i = 0, N = std::min(samples.size(), labels.size()); i<N; i++) {
                distance += std::pow(learned_function(samples[i]) - labels[i], 2);
            }
            distance = std::sqrt(distance);
            std::cout << " gamma: " << gamma << " nu: " << nu << " score: " << distance << std::endl;

            // Picking for gamma and nu the best value, that is the one minimizing the distance
            if (distance < scoresforCandidates) {
                candidateGamma = gamma;
                candidateNu = nu;
                scoresforCandidates = distance;
                std::cout << " ~~~ New candidate ! ~~~~ " << std::endl;
            }
        }
    }


    // From looking at the output of the above loop it turns out that a good value for nu
    // and gamma for this problem is 0.15625 for both.  So that is what we will use.

    // Now we train on the full set of data and obtain the resulting decision function.  We
    // use the value of 0.15625 for nu and gamma.  The decision function will return values
    // >= 0 for samples it predicts are in the +1 class and numbers < 0 for samples it
    // predicts to be in the -1 class.
    trainer.set_kernel(kernel_type(candidateGamma));//candidateGamma
    trainer.set_nu(candidateNu);
    typedef dlib::decision_function<kernel_type> dec_funct_type;
    typedef dlib::normalized_function<dec_funct_type> funct_type;

    // Here we are making an instance of the normalized_function object.  This object
    // provides a convenient way to store the vector normalization information along with
    // the decision function we are going to learn.
    funct_type learned_function;
    learned_function.normalizer = normalizer;  // save normalization information
    learned_function.function = trainer.train(samples, labels); // perform the actual SVM training and save the results

    // print out the number of support vectors in the resulting decision function
    std::cout << "\nnumber of support vectors in our learned_function is "
              << learned_function.function.basis_vectors.size() << std::endl;

    // Now let's try this decision_function on some samples we haven't seen before.
    sample_type sample;

    sample(0) = 0;
    sample(1) = 0;
    std::cout << "(0,0): the classifier output is " << learned_function(sample) << std::endl;

    sample(0) = 0;
    sample(1) = 1;
    std::cout << "(0,1): the classifier output is " << learned_function(sample) << std::endl;

    sample(0) = 1;
    sample(1) = 0;
    std::cout << "(1,0): the classifier output is " << learned_function(sample) << std::endl;

    sample(0) = 1;
    sample(1) = 1;
    std::cout << "(1,1): the classifier output is " << learned_function(sample) << std::endl;
}
