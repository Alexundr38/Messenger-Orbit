//
// Created by magofrays on 12/3/25.
//

#ifndef MESSENGERORBIT_NEWTONFORMULAPROXY_H
#define MESSENGERORBIT_NEWTONFORMULAPROXY_H
#include <fstream>

#include "NewtonFormula.h"
#include "../utils/PathResolver.h"

class NewtonFormulaProxy final : public NewtonFormula
{
    std::string filename;
    double start_date;
    std::ofstream csv_file;
public:
    NewtonFormulaProxy(const std::vector<SpaceObject*>& force_bodies, const std::string& object_name,
        const BodyState& start_state, SpiceDouble step, std::string filename)
        : NewtonFormula(force_bodies, object_name, BodyState(start_state.position, start_state.velocity, start_state.time/day), step/day)
    {
        start_date = start_state.time;
        filename = PathResolver::get_data(filename);
        csv_file.open(filename);
        csv_file << "time,pos_x,pos_y,pos_z,vel_x,vel_y,vel_z\n";

    }

    BodyState get_body_state(SpiceDouble tdb) override;

    ~NewtonFormulaProxy() override
    {
        if (csv_file.is_open()) {
            csv_file.close();
        }
    }
};


#endif //MESSENGERORBIT_NEWTONFORMULAPROXY_H