#include "Arduino.h"
#include "Test.h"
#include <Adafruit_ST7735.h>

int WIDTH;
int HEIGHT;

const char ERR[6] = "Error";
const char TEMP[12] = "Temperatur:";
const char HUM[13] = "Luftfeuchte:";
const char MIN[5] = "min:";
const char MAX[5] = "max:";
const char PERCENT[3] = " %";
const char DEGREE[3] = {' ', (char)247};

char sensorHumPrintout[5];
char sensorTempPrintout[5];
char minValue[5];
char maxValue[5];

float minT = 99;
float maxT = 0;
float minH = 100;
float maxH = 0;

Test::Test(Adafruit_ST7735 &tft)
{
    this->tft = &tft;
    HEIGHT = tft.height(); // Adafruit library handles display in vertical mode, so we have to chnage height/width here.
    WIDTH = tft.width();
}

void Test::begin()
{
    tft->initR(INITR_BLACKTAB);
    tft->setRotation(1);
}

void Test::printTemperature(float value)
{
    const float factor = HEIGHT / 50;

    String sensorTempVal = String(value);
    sensorTempVal.toCharArray(sensorTempPrintout, 5);

    tft->setTextColor(ST77XX_WHITE);
    tft->fillScreen(ST77XX_BLACK);
    tft->setTextSize(2);
    // write the text to the top left corner of the tft
    tft->setCursor(0, 0);
    tft->print(TEMP);

    // ste the font size very large for the loop
    tft->setTextSize(4);

    if (value >= 25)
    {
        tft->setTextColor(ST77XX_RED);
    }
    else if (value >= 20)
    {
        tft->setTextColor(ST77XX_GREEN);
    }
    else
    {
        tft->setTextColor(ST77XX_BLUE);
    }
    
    // print the sensor value
    tft->setCursor(0, 30);
    tft->print(sensorTempPrintout);
    tft->print(DEGREE);

    drawValueBar(value, factor);
    drawProgressBarBoarder();
    drawMinMax(minT, maxT);

    tft->setCursor(65, 100);
    tft->print(DEGREE);
    tft->setCursor(65, 115);
    tft->print(DEGREE);
}

void Test::printHumidity(float value)
{
    const float factor = HEIGHT / 100;

    String sensorHumVal = String(value);
    sensorHumVal.toCharArray(sensorHumPrintout, 5);

    tft->setTextColor(ST77XX_WHITE);
    tft->fillScreen(ST77XX_BLACK);
    tft->setTextSize(2);
    // write the text to the top left corner of the tft
    tft->setCursor(0, 0);
    tft->print(HUM);

    // ste the font size very large for the loop
    tft->setTextSize(4);

    if (value >= 40 && value <= 60)
    {
        tft->setTextColor(ST77XX_GREEN);
    }
    else
    {
        tft->setTextColor(ST77XX_RED);
    }
    // print the sensor value
    tft->setCursor(0, 30);
    tft->print(sensorHumPrintout);
    tft->print(PERCENT);

    drawValueBar(value, factor);
    drawProgressBarBoarder();
    drawMinMax(minH, maxH);

    tft->setCursor(65, 100);
    tft->print(PERCENT);
    tft->setCursor(65, 115);
    tft->print(PERCENT);
}

void Test::drawProgressBarBoarder()
{
    tft->setTextColor(ST77XX_WHITE);
    tft->drawRect(WIDTH - 10, HEIGHT - 123, 8, 118, ST77XX_WHITE);
}

void Test::drawScale()
{
    tft->setTextSize(1);
    tft->setCursor(5, 20);
    tft->print("50C");

    tft->setCursor(70, 20);
    tft->print("100%");

    tft->drawFastVLine(35, 20, 100, ST77XX_WHITE);
    tft->drawFastHLine(27, 20, 8, ST77XX_WHITE);
    tft->drawFastHLine(30, 44, 5, ST77XX_WHITE);
    tft->drawFastHLine(27, 69, 8, ST77XX_WHITE);
    tft->drawFastHLine(30, 94, 5, ST77XX_WHITE);
    tft->drawFastHLine(27, 119, 8, ST77XX_WHITE);

    tft->drawFastVLine(105, 20, 100, ST77XX_WHITE);
    tft->drawFastHLine(97, 20, 8, ST77XX_WHITE);
    tft->drawFastHLine(100, 44, 5, ST77XX_WHITE);
    tft->drawFastHLine(97, 69, 8, ST77XX_WHITE);
    tft->drawFastHLine(100, 94, 5, ST77XX_WHITE);
    tft->drawFastHLine(97, 119, 8, ST77XX_WHITE);
}

void Test::drawValueBar(float value, float factor)
{

    for (int i = HEIGHT - (value * factor); i < HEIGHT - 8; i = i + 8)
    {
        for (int j = 5; j < 9; ++j)
        {
            tft->drawLine(WIDTH - j, i, WIDTH - j, i + 4, ST77XX_GREEN);
        }
    }
}

void Test::drawMinMax(float min, float max)
{
    String minimum = String(min);
    String maximum = String(max);
    minimum.toCharArray(minValue, 5);
    maximum.toCharArray(maxValue, 5);

    tft->setTextSize(1);
    tft->setTextColor(ST77XX_GREEN);
    tft->setCursor(5, 100);
    tft->print(MIN);
    tft->setCursor(40, 100);
    tft->print(minValue);
    tft->setTextColor(ST77XX_RED);
    tft->setCursor(5, 115);
    tft->print(MAX);
    tft->setCursor(40, 115);
    tft->print(maxValue);
}

void Test::setMinMaxValues(float temp, float hum)
{
    if (temp < minT)
    {
        minT = temp;
    }
    if (temp > maxT)
    {
        maxT = temp;
    }
    if (hum < minH)
    {
        minH = hum;
    }
    if (hum > maxH)
    {
        maxH = hum;
    }
}

void Test::drawTempBar(int nPer)
{
    tft->fillRect(40, 20 + (100 - nPer), 20, nPer, ST77XX_GREEN);
}

void Test::drawHumBar(int nPer)
{
    tft->fillRect(110, 20 + (100 - nPer), 20, nPer, ST77XX_GREEN);
}

void Test::clearScreen()
{
    tft->fillScreen(ST77XX_BLACK);
}

bool Test::checkError(float h, float t, bool isError)
{
     // Check if any reads failed and exit early (to try again)->
    if (isnan(h) || isnan(t))
    {
        if (!isError)
        {
            tft->fillScreen(ST77XX_RED);
            tft->setTextSize(4);
            tft->setCursor(10, 30);
            tft->print("ERROR");
            return true;
        }
        return false;
    }
}

