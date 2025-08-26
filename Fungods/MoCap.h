#pragma once

#include "RTProtocol.h"
#include "RTPacket.h"
#include "EVENTS.h"
#include "PARAMS.h"
#include <SDL3/SDL.h>
#include <iostream>

#ifdef _WIN32
#define sleep Sleep
#else
#include <unistd.h>
#endif

class MoCap
{
private:
    CRTProtocol rtProtocol;
    const char serverAddr[10] = "127.0.0.1";
    const unsigned short basePort = 22222;
    bool dataAvailable = false;
    bool streamFrames = false;
    unsigned short udpPort = 6734;
    Uint32 m_event = NULL;
    bool m_spawnReady = true;

    // hard gecodeerde coördinaten van de fysieke randen
    const float m_L = 880;
    const float m_R = -1350;
    const float m_T = -450;
    const float m_B = 1800;

public:
    MoCap(Uint32 event) : m_event(event) {}
    ~MoCap() {
        rtProtocol.StopCapture();
        rtProtocol.Disconnect();
    }
    void stop() {
        rtProtocol.StopCapture();
        rtProtocol.Disconnect();
    }
    void process();
};

