//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_DECISION_TREE_H
#define TUTORIALS_DECISION_TREE_H


#include "split_field.h"
#include <vector>
#include <ai/datasets/DLibSplits.h>

template <typename impurity_function>
struct decision_tree {
    std::vector<metric_row>  rows;              ///<@ all the rows that are currently associated to the current elemtn
    std::string              class_field;       ///<@ the field that contains the class names
    double                   positive_class;  ///<@ The class among the @|\mathcal{Y}|@ that is going to provide the positive example
    std::vector<std::string> table_schema;      ///<@ the columns' names except from the class field
    struct split_field       root;              ///<@ decision associated to the current root
    std::vector<decision_tree>  children;       ///<@ number of children in the decision tree (in this implementation, only two children)
    impurity_function        functions;         ///<@ impurity function function that is used to do compute the gain

    decision_tree() = default;
    ~decision_tree() = default;
    decision_tree(const decision_tree<impurity_function>& x) = default;
    decision_tree& operator=(const decision_tree<impurity_function>& x) = default;

    /**
     * Populating the root of the tree with all the samples from the dataset
     * @param populator         Element containing all the initial examples
     * @param positive_class          The target class that is now considered to be the positive example
     */
    decision_tree(DLib_Splits& populator, double positive_class) {
        table_schema = populator.colnames;
        class_field = table_schema.front(); // The class is defined as the first attribute in the schema
        table_schema.erase(table_schema.begin()); //Removing the classification attribute from the predictors
        this->positive_class = positive_class;

        // Iterating over all the possible rows
        size_t N = std::min(populator.training_input.size(), populator.training_labels.size());
        for (size_t i = 0; i<N; i++) {
            metric_row row;

            // Converting each vector into a attribute-value row
            auto& matrixRow = populator.training_input[i];
            size_t M = matrixRow.size();
            for (size_t j = 0; j<M; j++) {
                row[table_schema[j]] = matrixRow(j);
            }
            // Also, adding the class information
            row[class_field] = populator.training_labels[i];
            // Adding the current record to the set of all the records
            rows.emplace_back(row);
        }
    }

    /**
     * Select the best attribute with the best splitting point
     * @param parent_field:     We pass the parent's decision predicate information, so to skip a decision that is similar to the one of the parent
     */
    void bestField(struct split_field* parent_field = nullptr) {
        struct split_field f;                                       // Temporary decision predicate
        double maxEntropy = -std::numeric_limits<double>::max();    // Setting a low value of entropy, so that we can detect the maximum
        root.is_not_discriminative = true;

        // Get which is the best column providing the best separation
        for (const std::string& columnName : table_schema) {        // Iterating over all the possible attribute names
            split_field currentField;                               // Current field
            currentField.field_name = columnName;
            auto cp = bestFieldSplit(currentField);             // Using the outcome if we had a positive or a negative leaf via cp
            if (cp.first) {
                root.is_pos_leaf = true;
                root.is_not_discriminative = false;
                return;
            } else if (cp.second) {
                root.is_neg_leaf = true;
                root.is_not_discriminative = false;
                return;
            } else {                                                // If I'm still considering the general case where there is no perfect division, then
                if ((currentField.is_not_discriminative) || (root == currentField) || (parent_field != nullptr && (currentField == *parent_field)))
                    continue;                                       // Skip the current decision outcome if either makes one child empty, or if I make the same decision as the root (break the loop).
                else if (currentField.impurity_decrease > maxEntropy) { // Otherwise, consider this field as a candidate only if it maximizes the entropy from the former candidate.
                    root.is_not_discriminative = false;
                    maxEntropy = currentField.impurity_decrease;
                    f = currentField;
                }
            }
        }

        // f now contains the best table option for the field
        if (!root.is_not_discriminative) root = f;
    }

    /**
     * Expanding the current node into multiple subtrees up until a given distance is reached
     *
     * @param maxDepth          Maximum depth that needs to be reached
     * @param depth             Current iteration depth
     * @param type              Trailing string used for displaying the tree while generating it
     */
    void expand(int maxDepth, const int depth = 0, const std::string& type = "", struct split_field* parent_field = nullptr) {
        bestField(parent_field); // Computing the best field providing the split
        std::cout << std::string(depth*2, '.') << type;
        if ((root.is_pos_leaf || root.is_neg_leaf) || (depth >= maxDepth) || (root.is_not_discriminative)) {
            // Stopping condition: if I reached the maximum tree size, or if I reach a non-discriminative node, or if I reach a leaf.
            std::cout << " = " << ((root.is_pos_leaf) ? 100.0 : (root.is_neg_leaf ? 0.0 : root.precision * 100)) << "%" << std::endl;
        } else {
            // Generating two new childs
            children.emplace_back();
            children.emplace_back();
            decision_tree& left = children[0];
            decision_tree& right = children[1];
            // Setting to each child the same information as the root
            left.table_schema = right.table_schema = table_schema;
            left.class_field = right.class_field = class_field;
            left.positive_class = right.positive_class = positive_class;

            // Splitting the nodes into left and right child
            for (metric_row& row : rows) {
                if ((root.is_equal) ?
                (row[root.field_name] == root.splitting_point) : // Using the equality predicate
                (row[root.field_name] < root.splitting_point))   // Using the less-than predicate
                {
                    left.rows.emplace_back(row);
                } else {
                    right.rows.emplace_back(row);
                }
            }

            // Removing duplicated data
            rows.clear();
            if (depth > 0) table_schema.clear();

            // recursive calls
            std::cout << "[" << root.field_name << (root.is_equal ? '=' : '<') << root.splitting_point << "] ?" << std::endl;
            left.expand(maxDepth, depth+1, "lhs ", &root);
            right.expand(maxDepth, depth+1, "rhs ", &root);
        }
    }

    double class_probability(const dlib::matrix<double>& matrixRow) {
        // Converting the vector into a row representation
        metric_row row;
        size_t M = matrixRow.size();
        for (size_t j = 0; j<M; j++) {
            row[table_schema[j]] = matrixRow(j);
        }

        // Running the actual probability evaluation that the element belongs to the current class given the decision
        return class_probability(row);
    }

    /**
     * Estimating the hypothesis from the regression function
     * @tparam my_impurity_function     Custom impurity function associated to the decision tree
     * @param classes                   Vector containing all the decision trees for each one of the classes
     * @param row                       Data record represented as a vector over which the N classifiers should do the decision
     * @return
     */
    template <typename my_impurity_function> static std::pair<double, std::set<double>> classify(std::vector<struct decision_tree<my_impurity_function>>& classes, const dlib::matrix<double> row) {
        double score = -std::numeric_limits<double>::max();
        std::set<double> candidates;
        // Running similarly as the one-to-one classification problem for neuroal networks
        for (struct decision_tree<my_impurity_function>& classifier : classes) {
            double currentClass = classifier.positive_class;
            double prob = classifier.class_probability(row); // Running the regression function £$\tilde{h}$£
            if (prob > score) {
                score = prob;
                candidates.clear();
                candidates.emplace(currentClass);
            } else if (prob == score) {
                candidates.emplace(currentClass);
            }
        }

        return {score, candidates};
    }

private:
    double class_probability(metric_row& row) {
        // I need to stop the recursion if the current node is a leaf, and if therefore it has no children.
        if ((root.is_pos_leaf || root.is_neg_leaf) || (root.is_not_discriminative || children.empty())) {
            if (root.is_pos_leaf)
                return 1.0;               // If I reach this point, I will have a positive element for sure
            else if (root.is_neg_leaf)
                return 0.0;               // If I reach this point, I will have a negative element for sure
            else
                return root.precision;    // If I reach a non-discriminative node or just a leaf, then the probability is the ratio between the positive and negative examples in that leaf
        } else {
            // Running the decision function from the root information, and then deciding which is the element to visit next
            if ((root.is_equal) ? (row[root.field_name] == root.splitting_point) : (row[root.field_name] < root.splitting_point)) {
                return children[0].class_probability(row);
            } else {
                return children[1].class_probability(row);
            }
        }
    }

    /**
     * For a given attribute, determines which is the best value providing the greatest impurity decrease
     * @param field     Field containing the attribute's name, and containing (at the end) the value over which the split is performed
     * @return          <is_positive,is_negative> pair determining if the decision is able to target one of the two classes of interest.
     */
    std::pair<bool,bool> bestFieldSplit(struct split_field& field) {
        assert (field.field_name != class_field);
        std::set<double>           sorted_values;
        std::map<double, double>   entropy_precompute;
        std::map<double, double>   frequency;

        double maxGain = -std::numeric_limits<double>::max();
        double posRoot = 0.0;
        double negRoot;
        double rootImpurity;

        // Getting all the possible values for the given field name. In order to do so, we need to iterate over the rows
        for (metric_row& r : rows) {
            double r_field = r[field.field_name];
            double r_class = r[class_field];

            // check if we have already encountered the value r_field
            auto  it = entropy_precompute.find(r_field);

            // key already present in the map
            if (it != entropy_precompute.cend()) {
                if (r_class == positive_class) it->second+=1.0;	// count the number of positive examples for the class
                frequency.find(r_field)->second+=1.0;              // count the number of total examples for the value
            } else { // key not found
                if (r_class == positive_class)
                    entropy_precompute[r_field] = 1.0; // count the number of positive examples for the class
                frequency[r_field] = 1.0;               // count the number of total examples for the value
            }

            if (r_class == positive_class) posRoot++;
        }

        posRoot /= ((double)rows.size());   // Evaluating the positive coverage
        negRoot = 1.0 - posRoot;            // Evaluating the negative coverage

        // if the leaf has posFreq == 1, then this node will contain only the nodes that are positive examples: stop the iteration.
        if (posRoot == 1.0) {
            return {true, false};
        }
        // similarly, if   negFreq == 1, then this node will contain only the nodes that are negative examples: stop the iteration.
        if (negRoot == 1.0) {
            return {false, true};
        }

        // If there is only one value over which we can discriminate, then we cannot use the current variable to further determine the data
        if (entropy_precompute.empty()) {
            field.is_not_discriminative = true;
            return {false, false};
        } else if (entropy_precompute.size() == 1) {
            // We use the equality predicate if the class is only going to tear apart one single element from the whole set.
            field.is_not_discriminative = false;
            field.is_equal = true;
        }

        // Using the metric to compute the impurity of the root prior to any split
        rootImpurity = functions.node_iota(posRoot);

        // Computing the entropy for each possible value (in a discrete way)
        auto it = entropy_precompute.begin();
        if (!field.is_equal) it++; // Skipping the first value, only if I have more than one value
        while (it != entropy_precompute.end()) {
            double positiveLeft = 0.0;
            double sizeLeft    = 0.0;
            double positiveRight = 0.0;
            double sizeRight   = 0.0;

            // Getting all the elements that are less or equal than the current element in it->first
            for (auto it2 = entropy_precompute.begin(); it2 != entropy_precompute.end(); it2++) {
                ((it2->first < it->first) ? positiveLeft : positiveRight) += it2->second;
                ((it2->first < it->first) ? sizeLeft : sizeRight) += frequency[it2->first];
            }

            // Getting the frequencies for the left branch of the decision
            double freq_posLeft = sizeLeft == 0 ? 0.0 :  positiveLeft / sizeLeft;
            double eLeft_eps = functions.node_iota(freq_posLeft);

            // Getting the frequencies for the right branch of the decision
            double freq_posRight = sizeRight == 0 ? 0.0 : positiveRight / sizeRight;
            double eRight_eps = functions.node_iota(freq_posRight);

            // Computing the impurity once we know which is the
            double entropyPosterior =
                    functions.posterior(sizeLeft, rows.size(), freq_posLeft, freq_posRight);
            double impurity_decrease = rootImpurity - entropyPosterior;

            // Setting up the new decision point once we obtain a better impurity decrease
            if ((impurity_decrease > maxGain) && (field.splitting_point < it->first)) {
                field.splitting_point = it->first;
                field.impurity_decrease = impurity_decrease;
                field.precision = posRoot;
                maxGain = rootImpurity;
            }

            it++;
        }

        // field now contains the best splitting point for the given field name
        return {false, false};
    }



};


#endif //TUTORIALS_DECISION_TREE_H
