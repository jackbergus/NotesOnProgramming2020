//
// Created by giacomo on 29/01/2020.
//

#ifndef TUTORIALS_CLASHROYALEDATASETMATCH_H
#define TUTORIALS_CLASHROYALEDATASETMATCH_H


#include <string>
#include <dm/dataset/ClashRoyalePlayer.h>
#include <optional>
#include <limits>		// for csv.h and numeric_limtis
#include <csv.h>

/**
 * Match information concerning who was the winning player.
 */
struct ClashRoyaleDatasetMatch {
    std::string   type;   ///<@ Game match type
    unsigned char winner; ///<@
    std::string   utctime;///<@ Time when the match occured

    ClashRoyalePlayer       player1; ///<@ First player
    ClashRoyalePlayer       player2; ///<@ Second player

    // The [[nodiscard]] attribute can be used to indicate that the return value of a function shouldn't be ignored when you do a function call. If the return value is ignored, the compiler should give a warning on this.
    [[nodiscard]] const ClashRoyalePlayer& getWinner() const {  return winner ? player2 : player1; }
    [[nodiscard]] const ClashRoyalePlayer& getLoser()  const {  return winner ? player1 : player2; }

    /**
     * Parsing the current line from the CSV file
     * @param csvProjection     CSV file from which the current line is read
     * @return                  Structured representation of the CSV line
     */
    static std::optional<ClashRoyaleDatasetMatch> read(
            io::CSVReader<31, io::trim_chars<' ', '\t'>, io::no_quote_escape<'^'>>& csvProjection);

    // Printing the structured CSV row to an output stream
    friend std::ostream &operator<<(std::ostream &os, const ClashRoyaleDatasetMatch &match);
};

#endif //TUTORIALS_CLASHROYALEDATASETMATCH_H
