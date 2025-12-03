//
// Created by magofrays on 12/3/25.
//

#include "NewtonFormulaProxy.h"

BodyState NewtonFormulaProxy::get_body_state(SpiceDouble tdb)
{
    return NewtonFormula::get_body_state((tdb/day));
}
