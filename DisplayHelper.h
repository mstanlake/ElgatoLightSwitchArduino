#ifndef DISPLAYHELPER_H
#define DISPLAYHELPER_H
#include "LightStatus.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class DisplayHelper {
    public:
        DisplayHelper(LightStatus* _lightStatus);

        void init();
        void drawDisplay();

        void ClearDisplay();
        void changeSelection();
        int getCurrentSelection() { return current_selection;}
        bool isDisplayCleared() { return displayCleared; }

    private:
        void drawTemperatureLine();
        void drawSelectionBox();
        void drawBrightnessLine();
        void drawStatusLine();
        
        int current_selection;
        bool displayCleared;
        LightStatus* lightStatus;
        Adafruit_SSD1306 display = Adafruit_SSD1306();
};

#endif // DISPLAYHELPER_H