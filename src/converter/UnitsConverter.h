//
// Created by magofrays on 12/13/25.
//

#ifndef MESSENGERORBIT_UNITSCONVERTER_H
#define MESSENGERORBIT_UNITSCONVERTER_H

#include "../types/ExtendedBodyState.h"


class UnitsConverter
{
    public:
    static ExtendedBodyState toAuDay(const ExtendedBodyState& state);
    static ExtendedBodyState toAuDayWithoutTime(const ExtendedBodyState& state);
    static ExtendedBodyState toKmSecWithoutTime(const ExtendedBodyState& state);
    static ExtendedBodyState toKmSec(const ExtendedBodyState& state);
    static std::pair<SpiceDouble, ExtendedBodyState> toKmSec(std::pair<SpiceDouble, ExtendedBodyState> pair);
    static std::pair<SpiceDouble, ExtendedBodyState> toAuDay(std::pair<SpiceDouble, ExtendedBodyState> pair);
};


#endif //MESSENGERORBIT_UNITSCONVERTER_H