#include "WS2812.h"
#include <Arduino.h>

#define LED_PIN 18
#define LED_COUNT 16

WS2812_Simple leds(LED_COUNT, LED_PIN, RMT_CHANNEL_0);

void setup() {
    leds.begin();
    leds.setBrightness(200);
    leds.fill(255, 0, 0); 
    leds.show();
}

void loop() {
    leds.rainbowMove();
    leds.show();
    delay(50);
}
