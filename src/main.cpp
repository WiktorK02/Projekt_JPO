#include "WS2812.h"
#include <Arduino.h>

#define LED_PIN 18 
//modify it 
#define LED_COUNT 16 
//modify it 

using namespace leds; 

WS2812_Simple ledsws2812(LED_COUNT, LED_PIN, RMT_CHANNEL_0);

void setup() {
    ledsws2812.begin();
    ledsws2812.setBrightness(255);
    //inicialization
}

void loop() {
    ledsws2812.rainbowMove(); // random effect 
    ledsws2812.show();
    delay(50);
}
