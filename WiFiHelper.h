#ifndef WIFIHELPER_H
#define WIFIHELPER_H

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiClient.h>

#define MAX_JSON_SIZE 256
static char putRequestFormat[] = "PUT /elgato/lights HTTP/1.1\r\n"  \
                                 "Content-Type: application/json\r\n"  \
                                 "Content-Length: %d\r\n\r\n" \
                                 "%s\r\n";

class WiFiHelper {
  public:
    WiFiHelper(IPAddress ipAddress, int port);
    bool setupWifi();
    
    char* getElgatoLightData();
    void updateElgatoLight(char* json);
    
    private: 
    WiFiClient client;
    char rawJson[MAX_JSON_SIZE];
    IPAddress elgatoLightIp;
    int port;
    
    
    void printCurrentNet();
    void printMacAddress(byte mac[]);
    void printWiFiData();
    
    bool handleElgatoResponse();
    bool trimHttpHeaders();

};



#endif // WIFIHELPER_H