cmake_minimum_required(VERSION 3.24)
project(MessengerOrbit VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0
)

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

FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
)
FetchContent_Declare(
        qd
        GIT_REPOSITORY https://github.com/scibuilder/qd.git
        GIT_TAG master
)

FetchContent_MakeAvailable(eigen glfw glm cspice yaml-cpp qd)
find_package(OpenGL REQUIRED)

set(COMMON_SOURCES
        src/types/Vec3d.cpp
        src/types/BodyState.cpp
        src/types/BodyState.h
        src/space/SpaceObject.cpp
        src/space/SpaceObject.h
        src/utils/PropertiesReader.h
        src/utils/PathResolver.h
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
        src/simulation/SimulationBuilder.h
        src/simulation/RenderSimulationBuilder.cpp
        src/simulation/RenderSimulationBuilder.h
        src/render/RenderSystem.cpp
        src/render/RenderSystem.h
        src/render/Renderable.h
        src/render/RenderFunctions.cpp
        src/render/RenderFunctions.h
        src/render/Camera.cpp
        src/render/Camera.h
        src/render/CameraController.cpp
        src/render/CameraController.h
        src/utils/SpiceGuard.h
        src/types/Mat3d.cpp
        src/types/Mat3d.h
        src/space/NewtonFormulaProxy.cpp
        src/space/NewtonFormulaProxy.h
        src/utils/Spline.h
        src/utils/Spline.cpp
        src/utils/LightTimeSolver.cpp
        src/utils/LightTimeSolver.h
        src/utils/DoplerComputer.cpp
        src/utils/DoplerComputer.h
        src/utils/LSM.cpp
        src/utils/LSM.h
        src/types/StateVector.cpp
        src/types/StateVector.h
        src/utils/CSVReader.cpp
        src/utils/CSVReader.h
        src/simulation/SimulationTime.h
        src/simulation/MessengerSimulationBuilder.cpp
        src/simulation/MessengerSimulationBuilder.h
        src/converter/UnitsConverter.cpp
        src/converter/UnitsConverter.h
        src/simulation/MessengerSimulationAdapter.cpp
        src/simulation/MessengerSimulationAdapter.h
        src/types/ExtendedBodyState.cpp
        src/types/ExtendedBodyState.h
        src/utils/HighPrecision.h
        src/simulation/BoostSimulation.cpp
        src/simulation/BoostSimulation.h
        src/simulation/BoostSimulationBuilder.cpp
        src/simulation/BoostSimulationBuilder.h
        src/types/HighPrecVec3d.h
        src/types/HighPrecVec3d.h
)

add_library(MessengerCommon STATIC ${COMMON_SOURCES})

target_include_directories(MessengerCommon PUBLIC
        ${cspice_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
        ${eigen_SOURCE_DIR}
        ${yaml-cpp_SOURCE_DIR}/include    # ← ДОБАВЬТЕ
        ${yaml-cpp_BINARY_DIR}
        /usr/include
)

target_link_directories(MessengerCommon PUBLIC ${cspice_SOURCE_DIR}/lib)
target_link_libraries(MessengerCommon
        -l:cspice.a
        yaml-cpp
)


add_executable(MessengerOrbit src/testMessengerSimulation.cpp)
target_link_libraries(MessengerOrbit
        MessengerCommon
)

add_executable(MessengerOrbitRender src/main.cpp)
target_link_libraries(MessengerOrbitRender
        MessengerCommon
        OpenGL::OpenGL
        glfw
)
