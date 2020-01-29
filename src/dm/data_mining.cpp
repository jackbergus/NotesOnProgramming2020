//
// Created by giacomo on 29/01/2020.
//

#include <vector>
#include <csv.h>
#include <optional>
#include <ostream>
#include <iostream>
#include <dm/dataset/ClashRoyaleDatasetMatch.h>
#include <unordered_set>
#include <utils/hashing.h>
#include <algorithm>
#include <string>

#include <set>

/**
 * Defining an ordering over vectors following the lexicographical order
 * @tparam T    Type associated to the container
 * @tparam U    Type associated to the content
 */
ForAll2(T,U) struct LexicographicalOrder {
    std::less<U> lesscmp;   // Default comparator for the content
    bool operator()(const T& lhs, const T& rhs) const {
        return compare(lhs, rhs, 0);
    }
private:
    bool compare(const T& lhs, const T& rhs, size_t idx) const {
        if (idx == lhs.size()) {
            return !(idx == rhs.size());
        } else {
            if (idx == rhs.size()) {
                return false;
            } else {
                if (lesscmp(lhs[idx], rhs[idx])) {
                    return true;
                } else if (lesscmp(rhs[idx], lhs[idx])) {
                    return false;
                } else {
                    return compare(lhs, rhs, idx+1);
                }
            }
        }
    }
};

using STLexic = struct LexicographicalOrder<Transaction<std::string>, std::string>;
using DeckSet = std::set<Transaction<std::string>, STLexic>;

int main(void) {
    // Loading the Clash Royale dataset. Even though we have more than 200 dimensions, we are only interested in a few of them
    // This file doesn't fit as it is in GitHub. So, pick the cr_data_1535999786739.zip file from the dataset_clashroyale
    // submodule, and unzip it in 'data/cr_data_1535999786739.csv'
    std::string path = "data/cr_data_1535999786739.csv";
    io::CSVReader<31, io::trim_chars<' ', '\t'>, io::no_quote_escape<'^'>> clash_royale{path};
    clash_royale.read_header(io::ignore_extra_column, "type", "winner", "utctime", "name_P1", "tag_P1", "clan_P1", "startTrophies_P1", "crownsEarned_P1", "human_deck_P1", "strenght_P1_0", "strenght_P1_1", "strenght_P1_2", "strenght_P1_3", "strenght_P1_4", "strenght_P1_5", "strenght_P1_6", "strenght_P1_7", "name_P2", "tag_P2", "clan_P2", "startTrophies_P2", "crownsEarned_P2", "human_deck_P2", "strenght_P2_0", "strenght_P2_1", "strenght_P2_2", "strenght_P2_3", "strenght_P2_4", "strenght_P2_5", "strenght_P2_6", "strenght_P2_7");

    std::vector<Transaction<std::string>> transactions;
    {
        DeckSet winning_deck, losing_deck;

        // Reading all the elements
        std::cout << "Reading and projecting all the battles from the dataset..." << std::endl;
        bool continueReading = true;
        do {
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

        transactions.resize(winning_deck.size());
        std::copy(winning_deck.begin(), winning_deck.end(), std::back_inserter(transactions));
    }
    std::cout << std::endl << std::endl;

    size_t minimum_support_threshold = 10000; // The pattern is frequent, and therefore supported by at least 10,000 rounds
    std::cout << "Feeding the FPGrowth Algorithm..." << std::endl;

    std::set<Pattern<std::string>> patterns;
    {
        const FPTree<std::string> fptree{ transactions, minimum_support_threshold };
        patterns = fptree_growth( fptree );
    }
    std::cout << patterns.size() << " patterns have been mined! " << std::endl;


}