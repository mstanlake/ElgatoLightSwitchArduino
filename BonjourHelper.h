#ifndef Bonjour_Helper_H
#define Bonjour_Helper_H

// The version of Ethernet Bonjour we are using is a forked version that used WifiUDP vs EthernetUDP.
#include <EthernetBonjour.h>
#include <WiFi101.h>
#include <SPI.h>

class BonjourHelper
{

    public:
        BonjourHelper();
        void init();
        IPAddress getLightIP();
        unsigned short getPort();
        bool isLightFound() {return lightFound; };
        void loop();
        


    public:         
        IPAddress lightIp;
        unsigned short port;
        bool lightFound;
};

extern BonjourHelper bonjourHelper;


#endif // Bonjour_Helper_H