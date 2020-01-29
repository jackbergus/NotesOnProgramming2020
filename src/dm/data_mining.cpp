//
// Created by giacomo on 29/01/2020.
//

#include <csv.h>
#include <optional>
#include <ostream>
#include <iostream>
#include <dm/dataset/ClashRoyaleDatasetMatch.h>

int main(void) {
    // Loading the Clash Royale dataset. Even though we have more than 200 dimensions, we are only interested in a few of them
    std::string path = "data/cr_data_1535999786739.csv";
    io::CSVReader<31, io::trim_chars<' ', '\t'>, io::no_quote_escape<'^'>> clash_royale{path};
    clash_royale.read_header(io::ignore_extra_column, "type", "winner", "utctime", "name_P1", "tag_P1", "clan_P1", "startTrophies_P1", "crownsEarned_P1", "human_deck_P1", "strenght_P1_0", "strenght_P1_1", "strenght_P1_2", "strenght_P1_3", "strenght_P1_4", "strenght_P1_5", "strenght_P1_6", "strenght_P1_7", "name_P2", "tag_P2", "clan_P2", "startTrophies_P2", "crownsEarned_P2", "human_deck_P2", "strenght_P2_0", "strenght_P2_1", "strenght_P2_2", "strenght_P2_3", "strenght_P2_4", "strenght_P2_5", "strenght_P2_6", "strenght_P2_7");

    // Reading all the elements
    bool continueReading = true;
    do {
        auto x = ClashRoyaleDatasetMatch::read(clash_royale);
        if (x) {                                                // If it was possible to read the information from the file
            std::cout << x.value() << std::endl;
        } else {
            continueReading = false;                            // Otherwise, fail and stop reading
        }
    } while (continueReading);
}