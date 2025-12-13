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
        const BodyState& start_state, SpiceDouble step, std::string filename);
    NewtonFormulaProxy(const std::string& object_name, const BodyState& start_state, SpiceDouble step,
                       std::string filename);


    BodyState get_body_state(SpiceDouble tdb) override;
    // void set_current_body_state(const BodyState& body_state) override;

    ~NewtonFormulaProxy() override
    {
        if (csv_file.is_open()) {
            csv_file.close();
        }
    }
};


#endif //MESSENGERORBIT_NEWTONFORMULAPROXY_H