//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_METRIC_TABLE_H
#define TUTORIALS_METRIC_TABLE_H


#include "split_field.h"
#include <vector>
#include <ai/datasets/DLibSplits.h>

template <typename metric>
struct metric_table {
    std::vector<metric_row>  rows;
    std::string              class_field;       // the field that contains the class names
    double                   positive_example;  // value providing the exact example
    std::vector<std::string> table_schema;      // the columns' names except from the class field
    struct split_field       root;
    std::vector<metric_table>            children;
    metric                   functions;

    metric_table() = default;
    ~metric_table() = default;
    metric_table(const metric_table<metric>& x) = default;
    metric_table& operator=(const metric_table<metric>& x) = default;

    metric_table(DLib_Splits& populator, double positive) {
        table_schema = populator.colnames;
        class_field = table_schema.front();
        table_schema.erase(table_schema.begin());
        positive_example = positive;

        size_t N = std::min(populator.training_input.size(), populator.training_labels.size());
        for (size_t i = 0; i<N; i++) {
            metric_row row;

            auto& matrixRow = populator.training_input[i];
            size_t M = matrixRow.size();
            for (size_t j = 0; j<M; j++) {
                row[table_schema[j]] = matrixRow(j);
            }
            row[class_field] = populator.training_labels[i];
            rows.emplace_back(row);
        }
    }

    void bestField() {
        struct split_field f;
        double maxEntropy = -std::numeric_limits<double>::max();
        root.is_not_discriminative = true;

        for (const std::string& columnName : table_schema) {
            split_field currentField;
            currentField.field_name = columnName;
            auto cp = bestFieldSplit(currentField);
            if (cp.first) {
                root.is_pos_leaf = true;
                root.is_not_discriminative = false;
                return;
            } else if (cp.second) {
                root.is_neg_leaf = true;
                root.is_not_discriminative = false;
                return;
            } else {
                if (currentField.is_not_discriminative)
                    continue;
                else if (currentField.entropy_gain > maxEntropy) {
                    root.is_not_discriminative = false;
                    maxEntropy = currentField.entropy_gain;
                    f = currentField;
                }
            }
        }

        // f now contains the best table option for the field
        if (!root.is_not_discriminative)
            root = f;
    }

    void expand(int maxDepth, const int depth = 0, const std::string& type = "") {
        bestField();
        std::cout << std::string(depth*2, '.') << type;
        if ((root.is_pos_leaf || root.is_neg_leaf) || (depth == maxDepth) || (root.is_not_discriminative)) {
            std::cout << " = " << ((root.is_pos_leaf) ? 100.0 : (root.is_neg_leaf ? 0.0 : root.precision * 100)) << "%" << std::endl;
        } else {
            children.emplace_back();
            children.emplace_back();
            metric_table& left = children[0];
            metric_table& right = children[1];
            left.table_schema = right.table_schema = table_schema;
            left.class_field = right.class_field = class_field;
            left.positive_example = right.positive_example = positive_example;

            for (metric_row& row : rows) {
                if ((root.is_equal) ? (row[root.field_name] == root.splitting_point) : (row[root.field_name] < root.splitting_point)) {
                    left.rows.emplace_back(row);
                } else {
                    right.rows.emplace_back(row);
                }
            }

            // not allowing to duplicate data
            rows.clear();
            if (depth > 0) table_schema.clear();

            // recursive calls
            std::cout << "[" << root.field_name << (root.is_equal ? '=' : '<') << root.splitting_point << "] ?" << std::endl;
            left.expand(maxDepth, depth+1, "lhs ");
            right.expand(maxDepth, depth+1, "rhs ");
        }
    }

    double class_probability(const dlib::matrix<double>& matrixRow) {
        metric_row row;
        size_t M = matrixRow.size();
        for (size_t j = 0; j<M; j++) {
            row[table_schema[j]] = matrixRow(j);
        }

        return class_probability(row);
    }

    template <typename metric2> static std::pair<double, std::set<double>> classify(std::vector<struct metric_table<metric2>>& classes, const dlib::matrix<double> row) {
        double score = -std::numeric_limits<double>::max();
        std::set<double> candidates;

        for (struct metric_table<metric2>& classifier : classes) {
            double currentClass = classifier.positive_example;
            double prob = classifier.class_probability(row);
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
                return 1.0;
            else if (root.is_neg_leaf)
                return 0.0;
            else
                return root.precision;
        } else {
            if ((root.is_equal) ? (row[root.field_name] == root.splitting_point) : (row[root.field_name] < root.splitting_point)) {
                return children[0].class_probability(row);
            } else {
                return children[1].class_probability(row);
            }
        }
    }

    std::pair<bool,bool> bestFieldSplit(struct split_field& field) {
        assert (field.field_name != class_field);
        std::set<double>           sorted_values;
        std::map<double, double>   entropy_precompute;
        std::map<double, double>   frequency;

        double maxGain = -std::numeric_limits<double>::max();
        double posRoot = 0.0;
        double negRoot = 0.0;
        double entropyRoot = 0.0;

        // Getting all the possible values for the given field name
        for (metric_row& r : rows) {
            double r_field = r[field.field_name];
            double r_class = r[class_field];

            // check if we have already encountered the value r_field
            auto  it = entropy_precompute.find(r_field);

            // key already present in the map
            if (it != entropy_precompute.cend()) {
                if (r_class == positive_example) it->second+=1.0;	// count the number of positive examples for the class
                frequency.find(r_field)->second+=1.0;              // count the number of total examples for the value
            } else { // key not found
                if (r_class == positive_example)
                    entropy_precompute[r_field] = 1.0; // count the number of positive examples for the class
                frequency[r_field] = 1.0;                                              // count the number of total examples for the value
            }

            if (r_class == positive_example) posRoot++;
        }

        posRoot /= ((double)rows.size());
        negRoot = 1.0 - posRoot;

        // if the leaf has posFreq == 1, then this node will contain only the nodes that are positive examples: stop the iteration.
        if (posRoot == 1.0) {
            return {true, false};
        }
        // similarly, if   negFreq == 1, then this node will contain only the nodes that are negative examples: stop the iteration.
        if (negRoot == 1.0) {
            return {false, true};
        }


        // If there is only one value over which we can discriminate, then we cannot use the current variable to further
        // determine the data
        if (entropy_precompute.empty()) {
            field.is_not_discriminative = true;
            return {false, false};
        } else if (entropy_precompute.size() == 1) {
            field.is_not_discriminative = false;
            field.is_equal = true;
        }

        entropyRoot = functions.metric_root(posRoot, negRoot); ///1.0; //- posRoot * xlogeps(posRoot) - negRoot * xlogeps(negRoot);

        // Computing the entropy for each possible value (in a discrete way)
        //std::cout << entropy_precompute.size() << std::endl;
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

#if 0
            if ((sizeLeft == 0.0) || (sizeRight == 0.0))
                continue;
#endif

            // Getting the frequencies for the
            double freq_posLeft = sizeLeft == 0 ? 0.0 :  positiveLeft / sizeLeft;
            double eLeft_eps = functions.subtree_part(freq_posLeft);

            double freq_posRight = sizeRight == 0 ? 0.0 : positiveRight / sizeRight;
            double eRight_eps = functions.subtree_part(freq_posRight);

            double entropyPosterior =
                    functions.posterior(sizeLeft, rows.size(), freq_posLeft, freq_posRight); ///(sizeLeft / ((double)rows.size())) * eLeft_eps + (sizeRight / ((double)rows.size())) * eRight_eps; //;
            double information_gain = entropyRoot - entropyPosterior;

            //std::cout << entropy << std::endl;
            if ((information_gain > maxGain) && (field.splitting_point < it->first)) {
                field.splitting_point = it->first;
                field.entropy_gain = information_gain;
                field.precision = posRoot;
                maxGain = entropyRoot;
            }

            it++;
        }

        // field now contains the best splitting point for the given field name
        return {false, false};
    }



};


#endif //TUTORIALS_METRIC_TABLE_H
