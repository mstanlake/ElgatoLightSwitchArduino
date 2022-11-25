
#include <WiFiUdp.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>

#include <SPI.h>
#include <WiFi101.h>
#include <EasyButton.h>

#include "WiFiHelper.h"
#include "DisplayHelper.h"
#include "LightStatus.h"


EasyButton easyButtonA(9);
EasyButton easyButtonB(6);
EasyButton easyButtonC(5);

int x =0;

LightStatus lightStatus;
DisplayHelper displayHelper = DisplayHelper(&lightStatus);
WiFiHelper wiFiHelper = WiFiHelper();

void onButtonAPressedFor2Seconds() {
  displayHelper.changeSelection(); 
  displayHelper.drawDisplay();
}

void onButtonAPressed() {
  lightStatus.ToggleLight();
  displayHelper.drawDisplay();
  wiFiHelper.updateElgatoLight(lightStatus.getJson());

  digitalWrite(13, lightStatus.isLightOn());
}

void onButtonBPressed() {
  if (displayHelper.getCurrentSelection() == 0)
  {
    lightStatus.IncreaseBrightness();
  } else {
    lightStatus.IncreaseTemperature();
  } 
  displayHelper.drawDisplay();
  wiFiHelper.updateElgatoLight(lightStatus.getJson());
}

void onButtonCPressed() {
  if (displayHelper.getCurrentSelection() == 0) {
    lightStatus.DecreaseBrightness();
  } else {
    lightStatus.DecreaseTemperature();
  }
  displayHelper.drawDisplay();
  wiFiHelper.updateElgatoLight(lightStatus.getJson());
}

// the setup function runs once when you press reset or power the board
void setup() {

  easyButtonA.begin();
  easyButtonA.onPressedFor(1000, onButtonAPressedFor2Seconds);
  easyButtonA.onPressed(onButtonAPressed);

  easyButtonB.begin();
  easyButtonB.onPressed(onButtonBPressed);
  
  easyButtonC.begin();
  easyButtonC.onPressed(onButtonCPressed);
  
  Serial.begin(9600);

#ifdef DEBUG  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif

  if (!wiFiHelper.setupWifi()) {
    Serial.println("WiFi not connected");
     // don't continue:
    while (true);
  }

    displayHelper.init();
  displayHelper.drawDisplay();

  
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);  
}

// the loop function runs over and over again forever
void loop() {
  bonjourHelper.loop();

  if (bonjourHelper.isLightFound() && !lightStatus.isDataInitialized())
  {
    Serial.println("Getting initial data from light");
    char* jsonLightData = wiFiHelper.getElgatoLightData();
    lightStatus.SetFromJson(jsonLightData);

    displayHelper.drawDisplay();
  }

  easyButtonA.read();
  easyButtonB.read();
  easyButtonC.read();

  if (!displayHelper.isDisplayCleared() && easyButtonA.releasedFor(3000) && easyButtonB.releasedFor(3000) && easyButtonC.releasedFor(3000) ){
    displayHelper.ClearDisplay();
    Serial.println("Display cleared");
  }
  
 
}








