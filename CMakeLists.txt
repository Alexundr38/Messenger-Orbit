cmake_minimum_required(VERSION 3.24)
project(MessengerOrbit VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
        cspice
        URL https://naif.jpl.nasa.gov/pub/naif/toolkit//C/PC_Linux_GCC_64bit/packages/cspice.tar.Z
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(cspice)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
)

FetchContent_MakeAvailable(cspice yaml-cpp)

add_executable(MessengerOrbit src/types/Vec3d.cpp
        src/types/BodyState.cpp
        src/types/BodyState.h
        src/space/SpaceObject.cpp
        src/space/SpaceObject.h
        src/utils/PropertiesReader.h
        src/utils/PathResolver.h
        src/main.cpp
        src/utils/SpaceObjectManager.cpp
        src/utils/SpaceObjectManager.h
        src/utils/TimeConverter.cpp
        src/utils/TimeConverter.h
        src/utils/PropertiesReader.cpp
        src/space/SpaceObjectEntity.cpp
        src/space/SpaceObjectEntity.h
        src/utils/PathResolver.cpp
        src/simulation/Simulation.cpp
        src/simulation/Simulation.h
        src/space/NewtonFormula.cpp
        src/space/NewtonFormula.h
        src/utils/Constants.h
        src/utils/IntegratingMethods3d.cpp
        src/utils/IntegratingMethods3d.h
        src/simulation/SimulationBuilder.h
)
target_include_directories(MessengerOrbit PRIVATE ${cspice_SOURCE_DIR}/include)
target_link_directories(MessengerOrbit PRIVATE ${cspice_SOURCE_DIR}/lib)
target_link_libraries(
        MessengerOrbit
        -l:cspice.a
        yaml-cpp
        m)