//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_CLASHROYALEDATASETMATCH_H
#define TUTORIALS_CLASHROYALEDATASETMATCH_H


#include <string>
#include <dm/dataset/ClashRoyalePlayer.h>
#include <optional>
#include <csv.h>

/**
 * Match information concerning who was the winning player.
 */
struct ClashRoyaleDatasetMatch {
    std::string   type;
    unsigned char winner;
    std::string   utctime;

    ClashRoyalePlayer       player1;
    ClashRoyalePlayer       player2;

    [[nodiscard]] const ClashRoyalePlayer& getWinner() const {  return winner ? player2 : player1; }
    [[nodiscard]] const ClashRoyalePlayer& getLoser()  const {  return winner ? player1 : player2; }

    static std::optional<ClashRoyaleDatasetMatch> read(
            io::CSVReader<31, io::trim_chars<' ', '\t'>, io::no_quote_escape<'^'>>& csvProjection);

    friend std::ostream &operator<<(std::ostream &os, const ClashRoyaleDatasetMatch &match);
};

#endif //TUTORIALS_CLASHROYALEDATASETMATCH_H
