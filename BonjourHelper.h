#ifndef Bonjour_Helper_H
#define Bonjour_Helper_H

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