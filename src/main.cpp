//
// Created by magofrays on 10/30/25.
//
#include <iostream>
#include "utils/SpaceObjectManager.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!" << std::endl;
    std::cout << SpaceObjectManager::get_body_state_at_time("2004-08-03T06:15:00", "EARTH");
}
