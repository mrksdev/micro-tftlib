#ifndef Test_h
#define Test_h

#include "Arduino.h"

#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

class Test
{
    public:
        Test(Adafruit_ST7735 &tft);
        void begin();
        void printTemperature(float);
        void printHumidity(float);
        void drawProgressBarBoarder();
        void drawScale();
        void setMinMaxValues(float, float);
        void drawTempBar(int);
        void drawHumBar(int);
        void clearScreen();
        bool checkError(float, float, bool);
    private:
        Adafruit_ST7735 *tft;
        int _pin;
        void drawValueBar(float, float);
        void drawMinMax(float, float);
};

#endif
