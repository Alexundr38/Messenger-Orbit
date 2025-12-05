//
// Created by magofrays on 10/31/25.
//

#ifndef MESSENGERORBIT_SPACEOBJECTMANAGER_H
#define MESSENGERORBIT_SPACEOBJECTMANAGER_H

#include <SpiceZdf.h>
#include <string>
#include "../types/BodyState.h"

class SpaceObjectManager
{
    std::string reference_frame;
    std::string aberration_correction;
    std::string observer_body;
    SpaceObjectManager();
    static SpaceObjectManager& get_instance();

    public:

    [[nodiscard]] static BodyState get_body_state_at_time(SpiceDouble tdb, const std::string& target_body);
    [[nodiscard]] static BodyState get_body_state_at_time(SpiceDouble tdb, const std::string& target_body, const std::string& observer_body);
    [[nodiscard]] static BodyState get_body_state_at_time(const std::string& utc, const std::string& target_body);
    [[nodiscard]] static SpiceDouble get_body_gm(const std::string& target_body);
    static SpiceDouble get_body_radius(const std::string& target_body);
    static BodyState get_DSN_state_at_time(SpiceDouble tdb, const std::string& dsn_id);
};


#endif //MESSENGERORBIT_SPACEOBJECTMANAGER_H