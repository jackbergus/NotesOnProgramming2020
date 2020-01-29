//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_DECKITEM_H
#define TUTORIALS_DECKITEM_H

#include <string>
#include <rang.hpp>

/**
 * Describing a card within a player's deck
 */
struct DeckItem {
    std::string name;       ///< Card name
    double strength;        ///< Strength associated to the card

    DeckItem() = default;
    DeckItem(const DeckItem& x) = default;
    DeckItem& operator=(DeckItem& x) = default;

    bool operator<(const DeckItem &rhs) const;
    bool operator>(const DeckItem &rhs) const;
    bool operator<=(const DeckItem &rhs) const;
    bool operator>=(const DeckItem &rhs) const;
    friend std::ostream &operator<<(std::ostream &os, const DeckItem &item);
};


#endif //TUTORIALS_DECKITEM_H
