#ifndef LightStatus_H
#define LightStatus_H

#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "WiFiHelper.h"

class LightStatus
{
public:
	LightStatus();
    
    void SetFromJson(char* rawJson);

    bool isLightOn();
    bool ToggleLight();
    int GetBrightness();
    int GetTemperature();

    int IncreaseBrightness();
    int DecreaseBrightness();

    int IncreaseTemperature();
    int DecreaseTemperature();

    bool isDataInitialized() {return dataInitialized;} ;

    char* getJson();

private:
    StaticJsonDocument<MAX_JSON_SIZE> jsonDoc;
    char serializedJson[MAX_JSON_SIZE];
    bool dataInitialized;

    bool lightOn;
    int brightness;
    int temperature;
    int tempK;
};

#endif /* LIGHTSTATUS_H */
