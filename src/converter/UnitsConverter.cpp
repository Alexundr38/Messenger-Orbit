//
// Created by magofrays on 12/13/25.
//

#include "UnitsConverter.h"

BodyState UnitsConverter::toAuDay(const BodyState& state)
{
    return BodyState(state.position/au, state.velocity*(day/au), state.time/day);
}

BodyState UnitsConverter::toAuDayWithoutTime(const BodyState& state)
{
    return BodyState(state.position/au, state.velocity*(day/au), state.time);
}

BodyState UnitsConverter::toKmSecWithoutTime(const BodyState& state)
{
    return BodyState(state.position*au, state.velocity*(au/day), state.time);
}

BodyState UnitsConverter::toKmSec(const BodyState& state)
{
    return BodyState(state.position*au, state.velocity*(au/day), state.time*day);
}

std::pair<SpiceDouble, BodyState> UnitsConverter::toKmSec(std::pair<SpiceDouble, BodyState> pair)
{
    return std::make_pair(pair.first*day, toKmSec(pair.second));
}

std::pair<SpiceDouble, BodyState> UnitsConverter::toAuDay(std::pair<SpiceDouble, BodyState> pair)
{
    return std::make_pair(pair.first/day, toAuDay(pair.second));
}
