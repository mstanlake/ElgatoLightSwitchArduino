#include "DisplayHelper.h"

DisplayHelper::DisplayHelper(LightStatus* _lightStatus){
    lightStatus = _lightStatus;

}

void DisplayHelper::init(){

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C address 0x3C (for the 128x32)
  display.clearDisplay(); // Clear the display
  display.setTextSize(1); //Set our text size, size 1 correlates to 8pt font
  
  current_selection = 0;

}
void DisplayHelper::ClearDisplay(){
    displayCleared = true;
    display.clearDisplay();
    display.display();
       
}
void DisplayHelper::drawStatusLine(){
  display.setTextColor(WHITE);
  display.setCursor(2,0); //Start position for the font to appear
  display.print("Status: "); //Will display text on new line

  display.fillRect(46, 0, 100, 8, BLACK);
  display.setCursor(46, 0);
  display.println(lightStatus->isLightOn() ? "On" : "Off");
}

void DisplayHelper::drawBrightnessLine(){
  display.setTextColor(WHITE); //We're using a Monochrome OLED so color is irrelevant, pixels are binary.
  display.setCursor(2,11); //Start position for the font to appear
  display.println("Brightness: "); //Will display text on new line
  
  display.fillRect(70, 11, 30, 8, BLACK);
  display.setCursor(70, 11);
  display.setTextColor(WHITE);
  display.print(lightStatus->GetBrightness());
  display.println("%");
}

void DisplayHelper::drawSelectionBox(){
  if (current_selection == 0)
  {
    display.drawRect(0, 9, 120, 11, WHITE);
    display.drawRect(0, 20, 120, 11, BLACK);

  } else {
    display.drawRect(0, 9, 120, 11, BLACK);
    display.drawRect(0, 20, 120, 11, WHITE);
  }
}

void DisplayHelper::drawTemperatureLine(){
  display.setCursor(2,22); //Start position for the font to appear
  display.println("Temperature: "); //Will display text on new line

  display.fillRect(75, 21, 30, 8, BLACK);
  display.setCursor(75, 22);
  display.setTextColor(WHITE);
  display.println(lightStatus->GetTemperature());
}

void DisplayHelper::drawDisplay() {
  drawStatusLine();
  drawBrightnessLine();
  drawTemperatureLine();
  drawSelectionBox();
  displayCleared = false;

  display.display();
}

void DisplayHelper::changeSelection(){
    current_selection = !current_selection;
}