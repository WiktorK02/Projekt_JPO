#include "WS2812.h"
#include <Arduino.h>

//pins and LED counts
#define LED_PIN 18
#define LED_COUNT 16

//instance of WS2812_Simple for the LED strip
WS2812_Simple leds(LED_COUNT, LED_PIN, RMT_CHANNEL_0);

void setup() {
    // initialize the LED strip
    leds.begin();
    leds.setBrightness(255); // set brightness to 100%
}

void loop() {
    static int mode = 9;

    switch (mode) {
        case 0:
            // Fill the strip with solid red
            leds.fill(255, 0, 0);
            leds.show();
            delay(100);
            break;

        case 1:
            // Fill with a gradient from red to blue
            leds.fillGradient(255, 0, 0, 0, 0, 255);
            leds.show();
            delay(100);
            break;

        case 2:
            // Rainbow animation
            leds.rainbow();
            leds.show();
            delay(100);
            break;

        case 3:
            // Chase effect (green)
            leds.chase(0, 255, 0, 100);
            delay(100);
            break;

        case 4:
            // Blink all strip blue
            leds.blink(0, 0, 255, 500);
            break;

        case 5:
            // Breathing effect with purple color
            leds.breathing(128, 0, 128, 3000);
            delay(100);
            break;

        case 6:
            leds.setReverseOrder(true);
            leds.fill(255, 255, 0);
            leds.show();
            delay(100);
            leds.setReverseOrder(false); 
            break;

        case 7:
            // Test RGB vs GRB color order
            leds.setColorOrder(COLOR_ORDER_RGB);
            leds.fill(255, 0, 0); // red (RGB)
            leds.show();
            delay(100);

            leds.setColorOrder(COLOR_ORDER_GRB);
            leds.fill(255, 0, 0); //green (GRB)
            leds.show();
            delay(100);
            break;
        case 8:
            // moving rainbow effect
            leds.rainbowMove();
            leds.show();
            delay(10); 
            break;

        case 9:
            // moving gradient effect
            leds.gradientMove(255, 0, 0, 0, 0, 255); 
            leds.show();
            delay(50); 
            break;

        default:
            mode = -1; // restart loop
            break;
    }
    //mode++;
}