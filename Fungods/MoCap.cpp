#include "MoCap.h"

void MoCap::process() {
    while (true) {
        try
        {

            if (!rtProtocol.Connected())
            {
                if (!rtProtocol.Connect(serverAddr, basePort, &udpPort))
                {
                    printf("rtProtocol.Connect: %s\n\n", rtProtocol.GetErrorString());
                    sleep(1000);
                    stop();
                }

                unsigned int major, minor;
                if (rtProtocol.GetVersion(major, minor))
                {
                    printf("rtProtocol.Connect: RT Protocol Version %d.%d\n\n", major, minor);
                }

                std::string qtmVersion;
                if (rtProtocol.GetQTMVersion(qtmVersion))
                {
                    printf("rtProtocol.Connect: Connected to %s\n\n", qtmVersion.data());
                }
            }

            if (!dataAvailable)
            {
                if (!rtProtocol.Read6DOFSettings(dataAvailable))
                {
                    printf("rtProtocol.Read6DOFSettings: %s\n\n", rtProtocol.GetErrorString());
                    sleep(1);
                    stop();
                }
            }

            if (!streamFrames)
            {
                if (!rtProtocol.StreamFrames(CRTProtocol::EStreamRate::RateAllFrames, 0, udpPort, NULL, CRTProtocol::cComponent6d))
                {
                    printf("rtProtocol.StreamFrames: %s\n\n", rtProtocol.GetErrorString());
                    sleep(1);
                    stop();
                }
                streamFrames = true;

                printf("Starting to streaming 6DOF data\n\n");
            }

            CRTPacket::EPacketType packetType;

            if (rtProtocol.Receive(packetType, true) == CNetwork::ResponseType::success)
            {
                if (packetType == CRTPacket::PacketData)
                {
                    float fX, fY, fZ;
                    float rotationMatrix[9];

                    CRTPacket* rtPacket = rtProtocol.GetRTPacket();

                    //printf("Frame %d\n", rtPacket->GetFrameNumber());
                    //printf("======================================================================================================================\n");

                    for (unsigned int i = 0; i < rtPacket->Get6DOFBodyCount(); i++)
                    {
                        if (rtPacket->Get6DOFBody(i, fX, fY, fZ, rotationMatrix))
                        {
                            const char* pTmpStr = rtProtocol.Get6DOFBodyName(i);
                            if (pTmpStr)
                            {
                                // printf("%-12s ", pTmpStr);
                            }
                            else
                            {
                                printf("Unknown     ");
                            }
                            printf("Pos: %9.3f %9.3f %9.3f    Rot: %6.3f %6.3f\n",
                                fX, fY, fZ, rotationMatrix[0], rotationMatrix[5]);
                            if (fZ > -165 && m_spawnReady) {
                                if (m_event == NULL)
                                    return;

                                float x, y;
                                int ix, iy;
                                y = 1 - (abs(fX - m_B) / abs(m_T - m_B));
                                iy = int(y * GRID_HEIGHT);

                                x = (abs(fY - m_L) / abs(m_R - m_L));
                                ix = int(x * GRID_WIDTH);

                                SDL_Event event;
                                SDL_zero(event);
                                event.type = m_event;
                                event.user.code = MC_TAP;
                                event.user.data1 = &ix;
                                event.user.data2 = &iy;
                                SDL_PushEvent(&event);
                                m_spawnReady = false;
                            }
                            else if (fZ <= -165) {
                                m_spawnReady = true;
                                if (fZ > -500) {
                                    if (m_event == NULL)
                                        return;

                                    float x, y;
                                    int ix, iy;
                                    y = 1 - (abs(fX - m_B) / abs(m_T - m_B));
                                    iy = int(y * GRID_HEIGHT);

                                    x = (abs(fY - m_L) / abs(m_R - m_L));
                                    ix = int(x * GRID_WIDTH);

                                    SDL_Event event;
                                    SDL_zero(event);
                                    event.type = m_event;
                                    event.user.code = MC_HOVER;
                                    event.user.data1 = &ix;
                                    event.user.data2 = &iy;
                                    SDL_PushEvent(&event);
                                }
                            }



                        }
                    }
                    //printf("\n");
                }
            }
        }
        catch (std::exception& e)
        {
            printf("%s\n", e.what());
        }
    }
}