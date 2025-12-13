//
// Created by magofrays on 12/13/25.
//

#ifndef MESSENGERORBIT_UNITSCONVERTER_H
#define MESSENGERORBIT_UNITSCONVERTER_H

#include "../types/BodyState.h"
#include "../utils/Constants.h"

class UnitsConverter
{
    public:
    static BodyState toAuDay(const BodyState& state);
    static BodyState toAuDayWithoutTime(const BodyState& state);
    static BodyState toKmSecWithoutTime(const BodyState& state);
    static BodyState toKmSec(const BodyState& state);
    static std::pair<SpiceDouble, BodyState> toKmSec(std::pair<SpiceDouble, BodyState> pair);
    static std::pair<SpiceDouble, BodyState> toAuDay(std::pair<SpiceDouble, BodyState> pair);
};


#endif //MESSENGERORBIT_UNITSCONVERTER_H