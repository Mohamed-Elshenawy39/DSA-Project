#pragma once

// A central place for all enumerations to avoid circular dependencies.

// Enum for the type of mission
enum MissionType {
    MISSION_POLAR,
    MISSION_NORMAL,
    MISSION_DIGGING
};

// Enum for the type of rover
enum RoverType {
    ROVER_POLAR,
    ROVER_NORMAL,
    ROVER_DIGGING
};

enum ReqType {
    NEW_REQ,
    ABORT_REQ
};

