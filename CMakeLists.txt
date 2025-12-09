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

FetchContent_MakeAvailable(eigen glfw glm cspice yaml-cpp)

find_package(OpenGL REQUIRED)

add_executable(MessengerOrbit
        src/types/Vec3d.cpp
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
        src/simulation/SimulationBuilder.h
        src/simulation/MessengerSimulationBuilder.cpp
        src/simulation/MessengerSimulationBuilder.h
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
)

target_include_directories(MessengerOrbit PRIVATE
        ${cspice_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
        ${eigen_SOURCE_DIR}
)

target_link_directories(MessengerOrbit PRIVATE ${cspice_SOURCE_DIR}/lib)

target_link_libraries(
        MessengerOrbit
        OpenGL::OpenGL
        -l:cspice.a
        yaml-cpp
        glfw
        m
)