//
// Created by giacomo on 28/01/2020.
//

#ifndef TUTORIALS_STARCRAFTREPLAYDATASET_H
#define TUTORIALS_STARCRAFTREPLAYDATASET_H

#include <csv.h>
#include <optional>

struct StarcraftReplayDataset{
    unsigned long GameId, MaxTimeStamp;
    unsigned short league_index, age;
    unsigned int hours_per_week,  total_hours;
    double APM, SelectByHotkeys, AssignToHotkeys, UniqueHotkeys, MinimapAttacks, MinimapRightClicks, NumberOfPACs, GapBetweenPACs, ActionLatency, ActionsInPAC, TotalMapExplored, WorkersMade, UniqueUnitsMade, ComplexUnitsMade, ComplexAbilityUsed;

    StarcraftReplayDataset() = default;
    StarcraftReplayDataset(const StarcraftReplayDataset& copy ) = default;
    StarcraftReplayDataset& operator=(const StarcraftReplayDataset& rhs) = default;

    static std::optional<StarcraftReplayDataset> read(io::CSVReader<21>& lineReader);
    bool wasReadingSuccessful(io::CSVReader<21>& lineReader);
    friend std::ostream &operator<<(std::ostream &os, const StarcraftReplayDataset &dataset);
};


#endif //TUTORIALS_STARCRAFTREPLAYDATASET_H
