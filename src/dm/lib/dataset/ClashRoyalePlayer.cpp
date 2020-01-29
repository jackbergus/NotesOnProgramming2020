//
// Created by giacomo on 29/01/2020.
//

#include "dm/dataset/ClashRoyalePlayer.h"

std::ostream &operator<<(std::ostream &os, const ClashRoyalePlayer &player) {
    os << "Player '" << player.name << "'" << std::endl << " - Tag: " << player.tag << std::endl << " - Clan: " << player.clan << std::endl <<  " - Start Trophies: "
       << player.startTrophies << std::endl << " - Crowns Earned: " << player.crownsEarned << std::endl << " - Deck: " << std::endl;
    for (size_t i = 0; i<8; i++) {
        os << "\t\t" << player.cards[i] << std::endl;
    }
    return os;
}
