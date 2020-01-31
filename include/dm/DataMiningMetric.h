//
// Created by giacomo on 31/01/2020.
//

#ifndef TUTORIALS_DATAMININGMETRIC_H
#define TUTORIALS_DATAMININGMETRIC_H

#include <map>
#include <vector>
#include <string>

#include <dm/Rule.h>
#include <utils/orders/LexicographicalOrder.h>

using VTLexic = struct LexicographicalOrder<std::vector<std::string>, std::string>;

/**
 * This struct stores the association itemset/number_of_occurrences returned from the FPGrowth algorithm and returns
 * a set of data mining metrics
 */
struct DataMiningMetrics {

    std::map<std::vector<std::string>, unsigned long, VTLexic> f;///<  Storing the item-support information
    double sumAll = 0.0;                                         ///<  Sum of all the supports

    /**
     * Initialization via the output of the FPGrowth algorithm
     * @param S
     */
    DataMiningMetrics(const std::set<Pattern<std::string>>& S);

    /**
     * Support definition for each item
     * @param i
     * @return
     */
    size_t support(const std::vector<std::string>& i) const;

    /**
     * Rule support
     * @param r
     * @return
     */
    double support(const Rule& r) const;

    /**
     * Rule lift
     * @param r
     * @return
     */
    double lift(const Rule& r) const;
};

#endif //TUTORIALS_DATAMININGMETRIC_H
