//
// Created by magofrays on 12/3/25.
//

#ifndef MESSENGERORBIT_NEWTONFORMULAPROXY_H
#define MESSENGERORBIT_NEWTONFORMULAPROXY_H
#include "NewtonFormula.h"


class NewtonFormulaProxy final : public NewtonFormula
{
public:
    NewtonFormulaProxy(const std::vector<SpaceObject*>& force_bodies, const std::string& object_name,
        const BodyState& start_state, SpiceDouble step)
        : NewtonFormula(force_bodies, object_name, start_state, step)
    {
    }

    BodyState get_body_state(SpiceDouble tdb) override;

};


#endif //MESSENGERORBIT_NEWTONFORMULAPROXY_H