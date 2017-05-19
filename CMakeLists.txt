cmake_minimum_required(VERSION 3.6.2)
project(P3)

################################################################################
# Project version
################################################################################
set(P3_VERSION_MAJOR 0)
set(P3_VERSION_MINOR 1)
set(P3_VERSION_PATCH 0)
set(P3_VERSION "${P3_VERSION_MAJOR}.${P3_VERSION_MINOR}.${P3_VERSION_PATCH}")
message(STATUS "P3 version ${P3_VERSION}")

################################################################################
# Header files
################################################################################
include_directories(include)

################################################################################
# P3 library
################################################################################
add_library(P3 STATIC
    src/Configuration/Configuration.cpp
    src/Events/Event.cpp
    src/Runtime/BugFindingRuntime.cpp
    src/Runtime/Runtime.cpp
    src/Runtime/ActorRuntime.cpp
    src/Actors/Actor.cpp
    src/Actors/Machine.cpp
    src/Actors/MachineState.cpp
    src/Actors/Monitor.cpp
    src/Actors/MonitorState.cpp
    src/Actors/ActorId.cpp
    src/TestingServices/Engines/BugFindingEngine.cpp
    src/TestingServices/ExplorationStrategies/RandomStrategy.cpp
    src/TestingServices/Scheduling/BugFindingScheduler.cpp
    src/TestingServices/Scheduling/ActorInfo.cpp
    src/TestingServices/Statistics/TestReport.cpp
)

################################################################################
# Examples
################################################################################
add_executable(PingPong
    examples/PingPong/Client.cpp
    examples/PingPong/Environment.cpp
    examples/PingPong/Program.cpp
    examples/PingPong/Safety.cpp
    examples/PingPong/Server.cpp
)

target_link_libraries(PingPong P3)