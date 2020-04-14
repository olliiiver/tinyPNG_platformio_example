#if defined(ESP32)

#include "SPI.h"
#include <esp32_image.h>
#include "tinyPNG.h"
#include <iostream>
#include <stdio.h>
#include <Adafruit_ILI9341.h>

using namespace std;

tinyPNG png;

int y = 0;

// Change pins as desired
#define TFT_CS 15
#define TFT_DC 5
#define TFT_MOSI 13
#define TFT_CLK 14
#define TFT_RST 4
#define TFT_MISO 12

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup()
{
    int i=0;
    tft.begin();
    tft.setRotation(1);
    delay(500);
    Serial.begin(9600);
    try
    {
        png.setPNG(image, 30143);
        Serial.printf("Width: %d\n", png.getWidth());
        Serial.printf("Height: %d\n", png.getHeight());
        Serial.printf("Theoretical deflated size: %d\n", png.getDeflatedSize());
        png.process(
            [](unsigned char *line) {
                int x;
                uint16_t color;
                tft.startWrite();
                for (x = 0; x < png.getWidth(); x++)
                {
                    // https://stackoverflow.com/questions/22937080/32bit-rgba-to-16bit-bgr565-conversion 
                    color  =  line[(x * 3) + 2] >> 3; // blue
                    color |= (line[(x * 3) + 1] & 0xFC) << 3; // green
                    color |= (line[(x * 3) + 0]  & 0xF8) << 8; // red
                    tft.writePixel(x,y, color);
                }
                y++;
                tft.endWrite();
            });
    }
    catch (string s)
    {
        Serial.printf("%s\n", s.c_str());
        return;
    }
}

void loop()
{

}

#endif