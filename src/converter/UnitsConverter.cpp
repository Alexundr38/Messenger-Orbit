//
// Created by magofrays on 12/13/25.
//

#include "UnitsConverter.h"
#include "../utils/Constants.h"

ExtendedBodyState UnitsConverter::toAuDay(const ExtendedBodyState& state)
{
    return ExtendedBodyState(state.position/au, state.velocity*(day/au), state.time/day);
}

ExtendedBodyState UnitsConverter::toAuDayWithoutTime(const ExtendedBodyState& state)
{
    return ExtendedBodyState(state.position/au, state.velocity*(day/au), state.time, state.jacobian);
}

ExtendedBodyState UnitsConverter::toKmSecWithoutTime(const ExtendedBodyState& state)
{
    return ExtendedBodyState(state.position*au, state.velocity*(au/day), state.time, state.jacobian);
}

ExtendedBodyState UnitsConverter::toKmSec(const ExtendedBodyState& state)
{
    return ExtendedBodyState(state.position*au, state.velocity*(au/day), state.time*day, state.jacobian);
}

std::pair<SpiceDouble, ExtendedBodyState> UnitsConverter::toKmSec(std::pair<SpiceDouble, ExtendedBodyState> pair)
{
    return std::make_pair(pair.first*day, toKmSec(pair.second));
}

std::pair<SpiceDouble, ExtendedBodyState> UnitsConverter::toAuDay(std::pair<SpiceDouble, ExtendedBodyState> pair)
{
    return std::make_pair(pair.first/day, toAuDay(pair.second));
}
