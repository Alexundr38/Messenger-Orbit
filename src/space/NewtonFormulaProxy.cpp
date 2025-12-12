//
// Created by magofrays on 12/3/25.
//

#include "NewtonFormulaProxy.h"

BodyState NewtonFormulaProxy::get_body_state(SpiceDouble tdb)
{
    auto state = NewtonFormula::get_body_state(tdb/day);
    if ((int)(tdb - start_date) % 60 == 0)
    {
        csv_file << std::scientific << std::uppercase << std::setprecision(15);
        csv_file << state.time << "," << state.position.x << ","
                    << state.position.y << ","
                    << state.position.z << ","
                    << state.velocity.x << ","
                    << state.velocity.y << ","
                    << state.velocity.z << "\n";
    }
    return state;
}
