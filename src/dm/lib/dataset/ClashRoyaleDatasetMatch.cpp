//
// Created by giacomo on 29/01/2020.
//

#include "dm/dataset/ClashRoyaleDatasetMatch.h"
#include <sstream>

std::optional<ClashRoyaleDatasetMatch>
ClashRoyaleDatasetMatch::read(io::CSVReader<31, io::trim_chars<' ', '\t'>, io::no_quote_escape<'^'>> &csvProjection) {
    ClashRoyaleDatasetMatch x;
    std::string human_deck_1; ///< This is the deck information: this will be later on splitted if the reading was fine
    std::string human_deck_2; ///< Same thing here

    // Reading some general match information
    bool test = csvProjection.read_row(x.type, x.winner, x.utctime,
            // Information regarding the first player
                                       x.player1.name, x.player1.tag, x.player1.clan, x.player1.startTrophies, x.player1.crownsEarned, human_deck_1, x.player1.cards[0].strength, x.player1.cards[1].strength, x.player1.cards[2].strength, x.player1.cards[3].strength, x.player1.cards[4].strength, x.player1.cards[5].strength, x.player1.cards[6].strength, x.player1.cards[7].strength,
            // Information regarding the second player
                                       x.player2.name, x.player2.tag, x.player2.clan, x.player2.startTrophies, x.player2.crownsEarned, human_deck_2, x.player2.cards[0].strength, x.player2.cards[2].strength, x.player2.cards[2].strength, x.player2.cards[3].strength, x.player2.cards[4].strength, x.player2.cards[5].strength, x.player2.cards[6].strength, x.player2.cards[7].strength);

    // Split the deck information in different cards only if the reading was successful
    if (test) {
        // Splitting the deck information in a more structured way
        {
            std::istringstream s1{human_deck_1};
            for (auto & card : x.player1.cards) s1 >> card.name;
        }
        {
            std::istringstream s2{human_deck_2};
            for (auto & card : x.player2.cards) s2 >> card.name;
        }
        // Now, we are ready to return the player
        return {x};
    } else {
        // No player was read
        return {};
    }
}

std::ostream &operator<<(std::ostream &os, const ClashRoyaleDatasetMatch &match) {
    os << "There was a " << match.type
       << " match, where the winner was the " << (match.winner ? "second" : "first")
       << " player at " << match.utctime << "! The match was between the following players: " << std::endl
       <<  " 1) " << match.player1 << std::endl << std::endl << " 2) " << match.player2;
    return os;
}
