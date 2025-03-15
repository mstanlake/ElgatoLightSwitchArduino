#include <SPI.h>
#include "LightStatus.h"
#include <WiFi101.h>


const int TemperatureMax = 344;
const int TemperatureMin = 144;
const int TempKMax = 2900;
const int TempKMin = 7000;
const int BrightnessMax = 100;
const int BrightnessMin = 0;


LightStatus::LightStatus()
{
    temperature = 300;
    brightness = 50;
    lightOn = 0;
    dataInitialized = false;
}

void LightStatus::SetFromJson(char* rawJson) {
    dataInitialized = true;
    Serial.println("Deserializing json");
   
    DeserializationError err = deserializeJson(jsonDoc, rawJson);

    if (err) {
        Serial.print("Json error: ");
        Serial.println(err.c_str());
        return;
    }
    jsonDoc["lights"][0]["on"] = true;
    lightOn = jsonDoc["lights"][0]["on"];
    brightness = jsonDoc["lights"][0]["brightness"];
    temperature= jsonDoc["lights"][0]["temperature"];

}

char* LightStatus::getJson() {
    jsonDoc["lights"][0]["on"] = lightOn ? 1 : 0;
    jsonDoc["lights"][0]["brightness"] = brightness;
    jsonDoc["lights"][0]["temperature"] = temperature;

    serializeJson(jsonDoc, serializedJson);

//    Serial.print("JSON: ");
//    Serial.println(serializedJson);
    return serializedJson;
}

bool LightStatus::isLightOn() {
    return lightOn;
}
bool LightStatus::ToggleLight(){
    lightOn = !lightOn;
    return lightOn;
}

// Increases Temp by 5%
int LightStatus::IncreaseTemperature() {
    Serial.println(temperature);
    temperature += 20;
    if (temperature > TemperatureMax) temperature = TemperatureMax;
    return temperature;
}

// Increases Temp by 5%
int LightStatus::DecreaseTemperature() {
    temperature -= 20;
    if (temperature < TemperatureMin) temperature = TemperatureMin;
    return temperature;
}

// Increases Temp by 10%
int LightStatus::DecreaseBrightness() {
    brightness -= 10;
    if (brightness < 0) brightness = 0;
    return brightness;
}
// Increases Temp by 10%
int LightStatus::IncreaseBrightness() {
    brightness += 10;
    if (brightness > 100) brightness = 100;
    return brightness;
}

int LightStatus::GetBrightness() {
    return brightness;
}

int LightStatus::GetTemperature() {
    return temperature;
}

