//
// Created by giacomo on 29/01/2020.
//

#include "dm/dataset/DeckItem.h"

bool DeckItem::operator<(const DeckItem &rhs) const {
    return strength < rhs.strength;
}

bool DeckItem::operator>(const DeckItem &rhs) const {
    return rhs < *this;
}

bool DeckItem::operator<=(const DeckItem &rhs) const {
    return !(rhs < *this);
}

bool DeckItem::operator>=(const DeckItem &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const DeckItem &item) {
    os << "Item: " << rang::style::bold << item.name  << rang::style::reset << " with strength = " << item.strength;
    return os;
}
