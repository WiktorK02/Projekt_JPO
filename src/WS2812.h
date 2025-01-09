#ifndef WS2812_H
#define WS2812_H

#include <driver/rmt.h>

// timing definitions for ws2812 (in 12.5ns tcs)
#define T0H 14   // 0 bit high time
#define T0L 34   // 0 bit low time
#define T1H 34   // 1 bit high time
#define T1L 14   // 1 bit low time

enum ColorOrder {
    COLOR_ORDER_GRB,
    COLOR_ORDER_RGB,
};

class WS2812_Simple {
public:
    WS2812_Simple(int ledCount, int ledPin, rmt_channel_t rmtChannel);
    void begin();

    void setPixelColor     (int index, uint8_t red, uint8_t green, uint8_t blue);
    void setPixelColor     (int index, uint32_t color); 
    uint32_t getPixelColor (int index);            
    void fill              (uint8_t red, uint8_t green, uint8_t blue); 
    void fillGradient      (uint8_t startRed, uint8_t startGreen, uint8_t startBlue, uint8_t endRed, uint8_t endGreen, uint8_t endBlue); 
    void setBrightness     (uint8_t brightness);
    void setReverseOrder   (bool reverse);
    void setColorOrder     (ColorOrder order);
    void rainbow();  
    void show();

    // Animations 
    void rainbowMove(); 
    void chase        (uint8_t red, uint8_t green, uint8_t blue, int delayMs); 
    void blink        (uint8_t red, uint8_t green, uint8_t blue, int delayMs);
    void breathing    (uint8_t red, uint8_t green, uint8_t blue, int durationMs); 
    void gradientMove (uint8_t startRed, uint8_t startGreen, uint8_t startBlue, uint8_t endRed, uint8_t endGreen, uint8_t endBlue);


private:
    int LED_COUNT;
    int LED_PIN;
    rmt_channel_t RMT_CHANNEL;  // Instance-specific RMT channel
    rmt_item32_t* led_data;     // Dynamically allocated LED_COUNT * 24 bits
    uint8_t brightness = 255;
    bool reverseOrder = false;  
    ColorOrder colorOrder = COLOR_ORDER_GRB; 
    uint32_t* pixelColors; 
    void rgbToRmt(uint8_t red, uint8_t green, uint8_t blue, rmt_item32_t* data);
    void applyColorOrder(uint8_t& red, uint8_t& green, uint8_t& blue);
};

#endif