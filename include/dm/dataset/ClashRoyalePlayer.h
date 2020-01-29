//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_CLASHROYALEPLAYER_H
#define TUTORIALS_CLASHROYALEPLAYER_H

#include <string>
#include "DeckItem.h"

/**
 * Describing a player participating in a match
 */
struct ClashRoyalePlayer {
    std::string name;               ///< Player's name
    std::string tag;                ///< Player's tag
    std::string clan;               ///< Player's clan
    size_t      startTrophies;      ///< Player's trophies before the match
    size_t      crownsEarned;       ///< How many crowns were earned by the player
    DeckItem    cards[8];           ///< Player's deck at the moment of the battle

    friend std::ostream &operator<<(std::ostream &os, const ClashRoyalePlayer &player);
};


#endif //TUTORIALS_CLASHROYALEPLAYER_H
