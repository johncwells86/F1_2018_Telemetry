#pragma once
#include <sys/types.h>
#include <cstring>
struct PacketHeader
{
    uint16_t    packetFormat;         // 2018 2
    uint8_t     packetVersion;        // Version of this packet type, all start from 1 1
    uint8_t     packetId;             // Identifier for the packet type, see below 1
    uint64_t    sessionUID;           // Unique identifier for the session 8
    float       sessionTime;          // Session timestamp 4
    uint32_t     frameIdentifier;      // Identifier for the frame the data was retrieved on 1
    uint8_t     playerCarIndex;       // Index of player's car in the array 1
};

// MOTION PACKET
//  The motion packet gives physics data for all the cars being driven. There is additional data for the car being driven with the goal of being able to drive a motion platform setup.
// N.B. For the normalised vectors below, to convert to float values divide by 32767.0f. 16-bit signed values are used to pack the data and on the assumption that direction values are always between -1.0f and 1.0f.
// Frequency: Rate as specified in menus
// Size: 1341 bytes
struct CarMotionData
{
    CarMotionData() {}
    CarMotionData(char buffer[]) {
        struct CarMotionData* p=this;
        unsigned char* charPtr=(unsigned char*)p;
        memset(&worldPosX, buffer[0], sizeof(worldPosX));
        memset(&worldPosY, buffer[3], sizeof(worldPosY));
        memset(&worldPosZ, buffer[7], sizeof(worldPosZ));
        memset(&worldVelX, buffer[11], sizeof(worldVelX));
        memset(&worldVelY, buffer[15], sizeof(worldVelY));
        memset(&worldVelZ, buffer[19], sizeof(worldVelZ));

    }
    float         worldPosX;           // World space X position
    float         worldPosY;           // World space Y position
    float         worldPosZ;           // World space Z position
    float         worldVelX;           // Velocity in world space X
    float         worldVelY;           // Velocity in world space Y
    float         worldVelZ;           // Velocity in world space Z
    int16_t       worldForwardDirX;         // World space forward X direction (normalised)
    int16_t       worldForwardDirY;         // World space forward Y direction (normalised)
    int16_t       worldForwardDirZ;         // World space forward Z direction (normalised)
    int16_t       worldRightDirX;           // World space right X direction (normalised)
    int16_t       worldRightDirY;           // World space right Y direction (normalised)
    int16_t       worldRightDirZ;           // World space right Z direction (normalised)
    float         gForceLateral;            // Lateral G-Force component
    float         gForceLongitudinal;       // Longitudinal G-Force component
    float         gForceVertical;           // Vertical G-Force component
    float         yaw;                      // Yaw angle in radians
    float         pitch;                    // Pitch angle in radians
    float         roll;                     // Roll angle in radians
};
struct PacketMotionData
{
    PacketHeader    header;               // Header

    CarMotionData   carMotionData[20];    // Data for all cars on track

    // Extra player car ONLY data
    float         suspensionPosition[4];       // Note: All wheel arrays have the following order:
    float         suspensionVelocity[4];       // RL, RR, FL, FR
    float         suspensionAcceleration[4];   // RL, RR, FL, FR
    float         wheelSpeed[4];               // Speed of each wheel
    float         wheelSlip[4];                // Slip ratio for each wheel
    float         localVelocityX;              // Velocity in local space
    float         localVelocityY;              // Velocity in local space
    float         localVelocityZ;              // Velocity in local space
    float         angularVelocityX;            // Angular velocity x-component
    float         angularVelocityY;            // Angular velocity y-component
    float         angularVelocityZ;            // Angular velocity z-component
    float         angularAccelerationX;        // Angular velocity x-component
    float         angularAccelerationY;        // Angular velocity y-component
    float         angularAccelerationZ;        // Angular velocity z-component
    float         frontWheelsAngle;            // Current front wheels angle in radians
};


// SESSION PACKET
// The session packet includes details about the current session in progress.
// Frequency: 2 per second
// Size: 147 bytes
struct MarshalZone
{
    float  zoneStart;   // Fraction (0..1) of way through the lap the marshal zone starts
    int8_t   zoneFlag;    // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
};

struct PacketSessionData
{
    PacketHeader    header;                 // Header

    uint8_t           weather;                // Weather - 0 = clear, 1 = light cloud, 2 = overcast
                                              // 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t            trackTemperature;       // Track temp. in degrees celsius
    int8_t            airTemperature;         // Air temp. in degrees celsius
    uint8_t           totalLaps;              // Total number of laps in this race
    uint16_t          trackLength;            // Track length in metres
    uint8_t           sessionType;            // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P
                                              // 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ
                                              // 10 = R, 11 = R2, 12 = Time Trial
    int8_t            trackId;                // -1 for unknown, 0-21 for tracks, see appendix
    uint8_t           era;                    // Era, 0 = modern, 1 = classic
    uint16_t          sessionTimeLeft;        // Time left in session in seconds
    uint16_t          sessionDuration;        // Session duration in seconds
    uint8_t           pitSpeedLimit;          // Pit speed limit in kilometres per hour
    uint8_t           gamePaused;             // Whether the game is paused
    uint8_t           isSpectating;           // Whether the player is spectating
    uint8_t           spectatorCarIndex;      // Index of the car being spectated
    uint8_t           sliProNativeSupport;    // SLI Pro support, 0 = inactive, 1 = active
    uint8_t           numMarshalZones;        // Number of marshal zones to follow
    MarshalZone     marshalZones[21];       // List of marshal zones – max 21
    uint8_t           safetyCarStatus;        // 0 = no safety car, 1 = full safety car
                                                // 2 = virtual safety car
    uint8_t          networkGame;              // 0 = offline, 1 = online
};

// LAP DATA PACKET
// The lap data packet gives details of all the cars in the session.
// Frequency: Rate as specified in menus
// Size: 841 bytes
struct LapData
{
    float       lastLapTime;           // Last lap time in seconds
    float       currentLapTime;        // Current time around the lap in seconds
    float       bestLapTime;           // Best lap time of the session in seconds
    float       sector1Time;           // Sector 1 time in seconds
    float       sector2Time;           // Sector 2 time in seconds
    float       lapDistance;           // Distance vehicle is around current lap in metres – could
                                         // be negative if line hasn’t been crossed yet
    float       totalDistance;         // Total distance travelled in session in metres – could
                                         // be negative if line hasn’t been crossed yet
    float       safetyCarDelta;        // Delta in seconds for safety car
    uint8_t       carPosition;           // Car race position
    uint8_t       currentLapNum;         // Current lap number
    uint8_t       pitStatus;             // 0 = none, 1 = pitting, 2 = in pit area
    uint8_t       sector;                // 0 = sector1, 1 = sector2, 2 = sector3
    uint8_t       currentLapInvalid;     // Current lap invalid - 0 = valid, 1 = invalid
    uint8_t       penalties;             // Accumulated time penalties in seconds to be added
    uint8_t       gridPosition;          // Grid position the vehicle started the race in
    uint8_t       driverStatus;          // Status of driver - 0 = in garage, 1 = flying lap
                                         // 2 = in lap, 3 = out lap, 4 = on track
    uint8_t       resultStatus;          // Result status - 0 = invalid, 1 = inactive, 2 = active
                                         // 3 = finished, 4 = disqualified, 5 = not classified
                                         // 6 = retired
};

struct PacketLapData
{
    PacketHeader    header;              // Header

    LapData         lapData[20];         // Lap data for all cars on track
};

// EVENT PACKET
// This packet gives details of events that happen during the course of the race.
// Frequency: When the event occurs
// Size: 25 bytes
struct PacketEventData
{
    PacketHeader    header;               // Header
    uint8_t           eventStringCode[4];   // Event string code, see above
};

// PARTICIPANTS PACKET
// This is a list of participants in the race. If the vehicle is controlled by AI, then the name will be the driver name. If this is a multiplayer game, the names will be the Steam Id on PC, or the LAN name if appropriate. On Xbox One, the names will always be the driver name, on PS4 the name will be the LAN name if playing a LAN game, otherwise it will be the driver name.
// Frequency: Every 5 seconds
// Size: 1082 bytes
struct ParticipantData
{
    uint8_t      aiControlled;           // Whether the vehicle is AI (1) or Human (0) controlled
    uint8_t      driverId;               // Driver id - see appendix
    uint8_t      teamId;                 // Team id - see appendix
    uint8_t      raceNumber;             // Race number of the car
    uint8_t      nationality;            // Nationality of the driver
    char       name[48];               // Name of participant in UTF-8 format – null terminated
                                         // Will be truncated with … (U+2026) if too long
};
struct PacketParticipantsData
{
    PacketHeader    header;            // Header

    uint8_t           numCars;           // Number of cars in the data
    ParticipantData participants[20];
};

// CAR SETUPS PACKET
// This packet details the car setups for each vehicle in the session. Note that in multiplayer games, other player cars will appear as blank, you will only be able to see your car setup and AI cars.
// Frequency: Every 5 seconds
// Size: 841 bytes
struct CarSetupData
{
    uint8_t     frontWing;                // Front wing aero
    uint8_t     rearWing;                 // Rear wing aero
    uint8_t     onThrottle;               // Differential adjustment on throttle (percentage)
    uint8_t     offThrottle;              // Differential adjustment off throttle (percentage)
    float     frontCamber;              // Front camber angle (suspension geometry)
    float     rearCamber;               // Rear camber angle (suspension geometry)
    float     frontToe;                 // Front toe angle (suspension geometry)
    float     rearToe;                  // Rear toe angle (suspension geometry)
    uint8_t     frontSuspension;          // Front suspension
    uint8_t     rearSuspension;           // Rear suspension
    uint8_t     frontAntiRollBar;         // Front anti-roll bar
    uint8_t     rearAntiRollBar;          // Front anti-roll bar
    uint8_t     frontSuspensionHeight;    // Front ride height
    uint8_t     rearSuspensionHeight;     // Rear ride height
    uint8_t     brakePressure;            // Brake pressure (percentage)
    uint8_t     brakeBias;                // Brake bias (percentage)
    float     frontTyrePressure;        // Front tyre pressure (PSI)
    float     rearTyrePressure;         // Rear tyre pressure (PSI)
    uint8_t     ballast;                  // Ballast
    float     fuelLoad;                 // Fuel load
};

struct PacketCarSetupData
{
    PacketHeader    header;            // Header
    CarSetupData    carSetups[20];
};

// CAR TELEMETRY PACKET
// This packet details telemetry for all the cars in the race. It details various values that would be recorded on the car such as speed, throttle application, DRS etc.
// Frequency: Rate as specified in menus
// Size: 1085 bytes
struct CarTelemetryData
{
    uint16_t    speed;                      // Speed of car in kilometres per hour
    uint8_t     throttle;                   // Amount of throttle applied (0 to 100)
    int8_t      steer;                      // Steering (-100 (full lock left) to 100 (full lock right))
    uint8_t     brake;                      // Amount of brake applied (0 to 100)
    uint8_t     clutch;                     // Amount of clutch applied (0 to 100)
    int8_t      gear;                       // Gear selected (1-8, N=0, R=-1)
    uint16_t    engineRPM;                  // Engine RPM
    uint8_t     drs;                        // 0 = off, 1 = on
    uint8_t     revLightsPercent;           // Rev lights indicator (percentage)
    uint16_t    brakesTemperature[4];       // Brakes temperature (celsius)
    uint16_t    tyresSurfaceTemperature[4]; // Tyres surface temperature (celsius)
    uint16_t    tyresInnerTemperature[4];   // Tyres inner temperature (celsius)
    uint16_t    engineTemperature;          // Engine temperature (celsius)
    float     tyresPressure[4];           // Tyres pressure (PSI)
};

struct PacketCarTelemetryData
{
    PacketHeader        header;                // Header
    CarTelemetryData    carTelemetryData[20];
    uint32_t              buttonStatus;         // Bit flags specifying which buttons are being
                                                // pressed currently - see appendices
};

// CAR STATUS PACKET
// This packet details car statuses for all the cars in the race. It includes values such as the damage readings on the car.
// Frequency: 2 per second
// Size: 1061 bytes
struct CarStatusData
{
    uint8_t       tractionControl;          // 0 (off) - 2 (high)
    uint8_t       antiLockBrakes;           // 0 (off) - 1 (on)
    uint8_t       fuelMix;                  // Fuel mix - 0 = lean, 1 = standard, 2 = rich, 3 = max
    uint8_t       frontBrakeBias;           // Front brake bias (percentage)
    uint8_t       pitLimiterStatus;         // Pit limiter status - 0 = off, 1 = on
    float       fuelInTank;               // Current fuel mass
    float       fuelCapacity;             // Fuel capacity
    uint16_t      maxRPM;                   // Cars max RPM, point of rev limiter
    uint16_t      idleRPM;                  // Cars idle RPM
    uint8_t       maxGears;                 // Maximum number of gears
    uint8_t       drsAllowed;               // 0 = not allowed, 1 = allowed, -1 = unknown
    uint8_t       tyresWear[4];             // Tyre wear percentage
    uint8_t       tyreCompound;             // Modern - 0 = hyper soft, 1 = ultra soft
                                            // 2 = super soft, 3 = soft, 4 = medium, 5 = hard
                                            // 6 = super hard, 7 = inter, 8 = wet
                                            // Classic - 0-6 = dry, 7-8 = wet
    uint8_t       tyresDamage[4];           // Tyre damage (percentage)
    uint8_t       frontLeftWingDamage;      // Front left wing damage (percentage)
    uint8_t       frontRightWingDamage;     // Front right wing damage (percentage)
    uint8_t       rearWingDamage;           // Rear wing damage (percentage)
    uint8_t       engineDamage;             // Engine damage (percentage)
    uint8_t       gearBoxDamage;            // Gear box damage (percentage)
    uint8_t       exhaustDamage;            // Exhaust damage (percentage)
    int8_t        vehicleFiaFlags;          // -1 = invalid/unknown, 0 = none, 1 = green
                                            // 2 = blue, 3 = yellow, 4 = red
    float       ersStoreEnergy;           // ERS energy store in Joules
    uint8_t       ersDeployMode;            // ERS deployment mode, 0 = none, 1 = low, 2 = medium
                                            // 3 = high, 4 = overtake, 5 = hotlap
    float       ersHarvestedThisLapMGUK;  // ERS energy harvested this lap by MGU-K
    float       ersHarvestedThisLapMGUH;  // ERS energy harvested this lap by MGU-H
    float       ersDeployedThisLap;       // ERS energy deployed this lap
};

struct PacketCarStatusData
{
    PacketHeader        header;            // Header
    CarStatusData       carStatusData[20];
};

struct NetworkPacket {
    NetworkPacket() {}
    NetworkPacket(const NetworkPacket& c) {
        header.packetFormat = c.header.packetFormat;    
        header.packetVersion = c.header.packetVersion;
        header.packetId = c.header.packetId;        
        header.sessionUID = c.header.sessionUID; 
        header.sessionTime = c.header.sessionTime;     
        header.frameIdentifier = c.header.frameIdentifier;
        header.playerCarIndex = c.header.playerCarIndex;
    }
  PacketHeader header;
  char data[2027];
};

