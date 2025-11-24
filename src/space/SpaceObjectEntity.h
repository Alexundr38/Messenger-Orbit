//
// Created by magofrays on 11/1/25.
//

#ifndef MESSENGERORBIT_SPACEOBJECTENTITY_H
#define MESSENGERORBIT_SPACEOBJECTENTITY_H

#include "SpaceObject.h"
#include "../utils/SpaceObjectManager.h"

class SpaceObjectEntity: public SpaceObject
{
public:
    void set_object_name(const std::string& object_name) override;
    explicit SpaceObjectEntity(const std::string& object_name);
    BodyState get_body_state(SpiceDouble tdb) override;
};


#endif //MESSENGERORBIT_SPACEOBJECTENTITY_H