# F1_2018_Telemetry
UDP telemetry reader for F1 2018 Game.


The main change for F1 2018 is the introduction of multiple packet types: each packet can now carry different types of data rather than having one packet which contains everything. A header has been added to each packet as well so that versioning can be tracked and it will be easier for applications to check they are interpreting the incoming data in the correct way.

Each packet has the following header:

```
struct PacketHeader
{
    uint16    m_packetFormat;         // 2018
    uint8     m_packetVersion;        // Version of this packet type, all start from 1
    uint8     m_packetId;             // Identifier for the packet type, see below
    uint64    m_sessionUID;           // Unique identifier for the session
    float     m_sessionTime;          // Session timestamp
    uint      m_frameIdentifier;      // Identifier for the frame the data was retrieved on
    uint8     m_playerCarIndex;       // Index of player's car in the array
};
```

Refer to [packets.h](src/packets.h) for additional details.
