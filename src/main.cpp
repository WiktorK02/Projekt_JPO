#include "WS2812.h"
#include <Arduino.h>

#define LED_PIN 18 
//modify it 
#define LED_COUNT 16 
//modify it 

WS2812_Simple leds(LED_COUNT, LED_PIN, RMT_CHANNEL_0);

void setup() {
    leds.begin();
    leds.setBrightness(255);
    //inicialization
}

void loop() {
    leds.rainbowMove(); // random effect 
    leds.show();
    delay(50);
}
