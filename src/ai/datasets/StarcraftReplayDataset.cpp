//
// Created by giacomo on 28/01/2020.
//

#include "ai/datasets/StarcraftReplayDataset.h"

std::optional <StarcraftReplayDataset> StarcraftReplayDataset::read(io::CSVReader<21> &lineReader) {
    struct StarcraftReplayDataset toReturn;
    if (toReturn.wasReadingSuccessful(lineReader)) {
        return {toReturn};
    } else {
        return {};
    }
}

bool StarcraftReplayDataset::wasReadingSuccessful(io::CSVReader<21> &lineReader) {
    bool test = lineReader.read_row(GameId, league_index, age, hours_per_week, total_hours, APM, SelectByHotkeys, AssignToHotkeys, UniqueHotkeys, MinimapAttacks, MinimapRightClicks, NumberOfPACs, GapBetweenPACs, ActionLatency, ActionsInPAC, TotalMapExplored, WorkersMade, UniqueUnitsMade, ComplexUnitsMade, ComplexAbilityUsed, MaxTimeStamp);
    if (!test) return test;
    assert(league_index <= 8);
    assert(APM <= 390);
    assert(SelectByHotkeys <= 1.0);
    assert(AssignToHotkeys <= 1.0);
    assert(UniqueHotkeys <= 1.0);
    assert(MinimapAttacks <= 1.0);
    assert(MinimapRightClicks <= 1.0);
    assert(NumberOfPACs <= 1.0);
    assert(GapBetweenPACs <= 238.0);
    assert(ActionLatency <= 177.0);
    assert (age <= 44.0);
    assert(hours_per_week <= 169.0);
    assert(total_hours <= 1.0e+06);
    assert(ActionsInPAC <=  186.0);
    assert(TotalMapExplored <= 1.0);
    assert(WorkersMade <= 1.0);
    assert(UniqueUnitsMade <= 1.0);
    assert(ComplexUnitsMade <= 1.0);
    assert(ComplexAbilityUsed <= 1.0);
    assert(MaxTimeStamp <= 389000.0);
    return test;
}

std::ostream &operator<<(std::ostream &os, const StarcraftReplayDataset &dataset) {
    os << "GameId: " << dataset.GameId << " MaxTimeStamp: " << dataset.MaxTimeStamp << " league_index: "
       << dataset.league_index << " age: " << dataset.age << " hours_per_week: " << dataset.hours_per_week
       << " total_hours: " << dataset.total_hours << " APM: " << dataset.APM << " SelectByHotkeys: "
       << dataset.SelectByHotkeys << " AssignToHotkeys: " << dataset.AssignToHotkeys << " UniqueHotkeys: "
       << dataset.UniqueHotkeys << " MinimapAttacks: " << dataset.MinimapAttacks << " MinimapRightClicks: "
       << dataset.MinimapRightClicks << " NumberOfPACs: " << dataset.NumberOfPACs << " GapBetweenPACs: "
       << dataset.GapBetweenPACs << " ActionLatency: " << dataset.ActionLatency << " ActionsInPAC: "
       << dataset.ActionsInPAC << " TotalMapExplored: " << dataset.TotalMapExplored << " WorkersMade: "
       << dataset.WorkersMade << " UniqueUnitsMade: " << dataset.UniqueUnitsMade << " ComplexUnitsMade: "
       << dataset.ComplexUnitsMade << " ComplexAbilityUsed: " << dataset.ComplexAbilityUsed;
    return os;
}
