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
    src/Runtime/BugFindingRuntime.cpp
    src/Runtime/Runtime.cpp
    src/Runtime/ActorRuntime.cpp
    src/Core/Actor.cpp
    src/Core/Machine.cpp
    src/Core/MachineState.cpp
    src/Core/Monitor.cpp
    src/Core/MonitorState.cpp
    src/Core/ActorId.cpp
    src/Core/Events/Event.cpp
    src/TestingServices/Engines/BugFindingEngine.cpp
    src/TestingServices/ExplorationStrategies/RandomStrategy.cpp
    src/TestingServices/Scheduling/BugFindingScheduler.cpp
    src/TestingServices/Scheduling/ActorInfo.cpp
    src/TestingServices/Statistics/TestReport.cpp
)

################################################################################
# Tests
################################################################################
add_library(TestFramework STATIC
    tests/Test.cpp
)

add_executable(Tests
    tests/Machines/GotoStateTest.cpp
)

target_link_libraries(Tests P3 TestFramework)

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

add_executable(BoundedAsync
    examples/BoundedAsync/BoundedAsync.cpp
    examples/BoundedAsync/Environment.cpp
    examples/BoundedAsync/Process.cpp
    examples/BoundedAsync/Scheduler.cpp
)

target_link_libraries(BoundedAsync P3)

if(MSVC)
    option(USE_RUNTIME_DLL "If on, the P3 static library will use MSVCRT[D].dll at runtime." ON)
    if(NOT USE_RUNTIME_DLL)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    endif()
endif()

install(TARGETS P3 
    DESTINATION lib
)

# Hack to also copy pdb file, if it exists.
if(WIN32)
    install(FILES $<TARGET_FILE_DIR:P3>/P3.pdb DESTINATION lib OPTIONAL)
endif()

install(DIRECTORY include DESTINATION .)
