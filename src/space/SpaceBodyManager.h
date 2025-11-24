//
// Created by magofrays on 10/31/25.
//

#ifndef MESSENGERORBIT_SPACEBODYMANAGER_H
#define MESSENGERORBIT_SPACEBODYMANAGER_H

#include <SpiceZdf.h>
#include <string>
#include "../types/BodyData.h"

class SpaceBodyManager
{
    std::string reference_frame;
    std::string aberration_correction;
    std::string observer_body;


    SpaceBodyManager();

    public:
    static SpaceBodyManager& get_instance();
    [[nodiscard]] BodyState get_body_state_at_time(SpiceDouble tdb, const std::string& target_body) const;
    [[nodiscard]] BodyState get_body_state_at_time(const std::string& utc, const std::string& target_body) const;
};


#endif //MESSENGERORBIT_SPACEBODYMANAGER_H