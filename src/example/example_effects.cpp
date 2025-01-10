#include "WS2812.h"
#include <Arduino.h>

#define LED_PIN 18
#define LED_COUNT 16

using namespace leds;
//instance of WS2812_Simple for the LED strip
WS2812_Simple ledws2812(LED_COUNT, LED_PIN, RMT_CHANNEL_0);

void setup() {
    // initialize the LED strip
    ledws2812.begin();
    ledws2812.setBrightness(255); // set brightness to 100%
}

void loop() {
    static int mode = 9;

    switch (mode) {
        case 0:
            // Fill the strip with solid red
            ledws2812.fill(255, 0, 0);
            ledws2812.show();
            delay(100);
            break;

        case 1:
            // Fill with a gradient from red to blue
            ledws2812.fillGradient(255, 0, 0, 0, 0, 255);
            ledws2812.show();
            delay(100);
            break;

        case 2:
            // Rainbow animation
            ledws2812.rainbow();
            ledws2812.show();
            delay(100);
            break;

        case 3:
            // Chase effect (green)
            ledws2812.chase(0, 255, 0, 100);
            delay(100);
            break;

        case 4:
            // Blink all strip blue
            ledws2812.blink(0, 0, 255, 500);
            break;

        case 5:
            // Breathing effect with purple color
            ledws2812.breathing(128, 0, 128, 3000);
            delay(100);
            break;

        case 6:
            ledws2812.setReverseOrder(true);
            ledws2812.fill(255, 255, 0);
            ledws2812.show();
            delay(100);
            ledws2812.setReverseOrder(false); 
            break;

        case 7:
            // Test RGB vs GRB color order
            ledws2812.setColorOrder(COLOR_ORDER_RGB);
            ledws2812.fill(255, 0, 0); // red (RGB)
            ledws2812.show();
            delay(100);

            ledws2812.setColorOrder(COLOR_ORDER_GRB);
            ledws2812.fill(255, 0, 0); //green (GRB)
            ledws2812.show();
            delay(100);
            break;
        case 8:
            // moving rainbow effect
            ledws2812.rainbowMove();
            ledws2812.show();
            delay(10); 
            break;

        case 9:
            // moving gradient effect
            ledws2812.gradientMove(255, 0, 0, 0, 0, 255); 
            ledws2812.show();
            delay(50); 
            break;

        default:
            mode = -1; // restart loop
            break;
    }
    mode++;
}
