//
// Created by giacomo on 29/01/2020.
//

#include <vector>
#include <csv.h>
#include <ostream>
#include <iostream>
#include <dm/dataset/ClashRoyaleDatasetMatch.h>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

#include <set>
#include <cmath>
#include <dm/DataMiningMetric.h>
#include <dm/RulesFromFrequentItemset.h>


#ifdef _WIN64
#ifndef F_OK
#define F_OK		(00)
#endif 
#include <iterator>						//inserter
#else
extern "C" {
#include <unistd.h>
}
#endif


int main(void) {
    // Loading the Clash Royale dataset. Even though we have more than 200 dimensions, we are only interested in a few of them
    // This file doesn't fit as it is in GitHub. So, pick the cr_data_1535999786739.zip file from the dataset_clashroyale
    // submodule, and unzip it in 'data/cr_data_1535999786739.csv'
    std::string path = "data/cr_data_1535999786739.csv";
    // File where we can store the previously computed frequent patterns by the FPGrowth algorithm
    std::string patterns_file = "data/mined_patterns.txt";

    std::set<Pattern<std::string>> patterns;
    // Checking if the file exists: if the file doesn't exist, then I need to recompute the frequent itemsets from the database
    if (!(access(patterns_file.c_str(), F_OK) != -1)) {
        // Setting ^ as an escape characeter, and using all the possible spaces to be trimmed.
        io::CSVReader<31, io::trim_chars<' ', '\t'>, io::no_quote_escape<'^'>> clash_royale{path};
        // Defining which are the relevant columns to be read, and ignoring the others
        clash_royale.read_header(io::ignore_extra_column, "type", "winner", "utctime", "name_P1", "tag_P1", "clan_P1", "startTrophies_P1", "crownsEarned_P1", "human_deck_P1", "strenght_P1_0", "strenght_P1_1", "strenght_P1_2", "strenght_P1_3", "strenght_P1_4", "strenght_P1_5", "strenght_P1_6", "strenght_P1_7", "name_P2", "tag_P2", "clan_P2", "startTrophies_P2", "crownsEarned_P2", "human_deck_P2", "strenght_P2_0", "strenght_P2_1", "strenght_P2_2", "strenght_P2_3", "strenght_P2_4", "strenght_P2_5", "strenght_P2_6", "strenght_P2_7");
        // Vector storing all the transactions read from the CSV file
        std::vector<Transaction<std::string>> transactions;
        {
            // Shorthand for a set of transaction sorted in lexicographical order
            using DeckSet = std::set<Transaction<std::string>, VTLexic>;
            // Separating the decks that wins from the decks that lose some tournaments
            DeckSet winning_deck, losing_deck;
            // Reading all the elements in the CSV file
            std::cout << "Reading and projecting all the battles from the dataset..." << std::endl;
            bool continueReading = true;
            do {
                // Read a line from the file if possible
                const auto x = ClashRoyaleDatasetMatch::read(clash_royale);
                if (x) {                                                // If it was possible to read the information from the file
                    winning_deck.emplace(x->getWinner().asFPTransaction());
                    losing_deck.emplace(x->getLoser().asFPTransaction());
                    // Load both the winning and the losing decks
                } else {
                    continueReading = false;                            // Otherwise, fail and stop reading
                }
            } while (continueReading);

            /*
             * Now, we want to obtain the set of decks that are always winning: that information will be then fed to the FPGrowth algorithm
             */
            std::cout << "Initial number of winning moves: " << winning_deck.size() << std::endl;
            std::cout << "Number of losing decks: " << losing_deck.size() << std::endl;
            {
                DeckSet c;
                std::set_difference(std::make_move_iterator(winning_deck.begin()),
                                    std::make_move_iterator(winning_deck.end()),
                                    losing_deck.begin(), losing_deck.end(),
                                    std::inserter(c, c.begin()));
                winning_deck.swap(c);
            }
            std::cout << "Resulting number of always winning decks: " << winning_deck.size() << std::endl;
            std::copy(winning_deck.begin(), winning_deck.end(), std::back_inserter(transactions));
        }
        std::cout << std::endl << std::endl;

        size_t minimum_support_threshold = 10000; // The pattern is frequent, and therefore supported by at least 10,000 rounds
        std::cout << "Feeding the FPGrowth Algorithm..." << std::endl;

        {
            std::cout << "a) creating the tree" << std::endl;
            const FPTree<std::string> fptree{ transactions, minimum_support_threshold };

            std::cout << "b) computing the most frequent patterns" << std::endl;
            patterns = fptree_growth( fptree );
        }

        {
            std::cout << "c) Saving all the patterns in the file as a backup" << std::endl;
            std::ofstream mined_patterns{patterns_file};
            for (const Pattern<std::string>& pattern : patterns) {
                mined_patterns << pattern.second << " : ";
                for (const std::string& item : pattern.first) {
                    mined_patterns << item << " ";
                }
                mined_patterns << std::endl;
            }
        }
    } else {
        // ... If the file exists, then:
        std::cout << "Patterns have been already mined in a previous computation: loading those from the file!" << std::endl;
        // Opening the existing file containing the patterns
        std::ifstream mined_patterns{patterns_file};

        // The variable containing each line from the file
        std::string line;

        // Reading all the lines
        while (std::getline(mined_patterns, line)) {
            std::istringstream iss(line);
            uint64_t frequency;
            std::string item;
            char delimiter;

            // The first element in the row is the frequency value and some delimiter.
            iss >> frequency >> delimiter;
            // Assertion on the delimiter separating the frequency from the items
            assert(delimiter = ':');

            // Storing all the elements in the pattern
            std::set<std::string> pattern;
            while (iss >> item) pattern.emplace(item);

            // Storing the resulting pair in the set
            Pattern<std::string> mined{pattern, frequency};
            patterns.emplace(mined);
        }
    }

    /*
     * Initializing the scoring functions using the mined patterns from the FPGrowth algorithm
     */
    DataMiningMetrics counter{patterns};
    size_t count = 0;

    // For each frequent itemset, generate a set of possible relevant rules
    for (const Pattern<std::string>& pattern : patterns) {
        if (pattern.first.size() <= 1) continue;                            // I cannot extract any significant rule from a singleton!

        RulesFromFrequentItemset rffi{counter};                         // Generate the top rule
        for (auto& result: rffi.generate_hypotheses(pattern)) {            // Generate the hypotheses containing a lift greater than one
            std::cout  << result << std::endl;                             // Printing each successful outcome
            count++;
        }
    }

    std::cout << std::endl << "~ We mined " << patterns.size() << " patterns " << std::endl;
    std::cout << "~ from which we generated " <<  count << " rules!" << std::endl;
}