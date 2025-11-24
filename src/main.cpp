//
// Created by magofrays on 10/30/25.
//
#include <iostream>
#include "space/SpaceBodyManager.h"

int main(int argc, char* argv[])
{
    auto space_body_manager = SpaceBodyManager::get_instance();
    std::cout << space_body_manager.get_body_state_at_time("2004-08-03T06:15:00", "EARTH");
}
