#include "WS2812.h"
#define RMT_CLK_DIV 2

namespace leds {
    
    WS2812_Simple::WS2812_Simple(int ledCount, int ledPin, rmt_channel_t rmtChannel)
        : LED_COUNT(ledCount), LED_PIN(ledPin), RMT_CHANNEL(rmtChannel) {
        led_data = new rmt_item32_t[LED_COUNT * 24];
        pixelColors = new uint32_t[LED_COUNT]; 
        for (int i = 0; i < LED_COUNT * 24; i++) {
            led_data[i].level0 = 1;
            led_data[i].level1 = 0;
            led_data[i].duration0 = T0L;
            led_data[i].duration1 = T0H;
        }
    }

    void WS2812_Simple::begin() {
        rmt_config_t config = RMT_DEFAULT_CONFIG_TX((gpio_num_t)LED_PIN, RMT_CHANNEL);
        config.clk_div = RMT_CLK_DIV;
        rmt_config(&config);
        rmt_driver_install(RMT_CHANNEL, 0, 0);
    }

    void WS2812_Simple::setBrightness(uint8_t b) {
        brightness = b;
    }

    void WS2812_Simple::setPixelColor(int index, uint8_t red, uint8_t green, uint8_t blue) {
        if (index < 0 || index >= LED_COUNT) return;
        applyColorOrder(red, green, blue); 
        red = (red * brightness) / 255;
        green = (green * brightness) / 255;
        blue = (blue * brightness) / 255;
        pixelColors[index] = (green << 16) | (red << 8) | blue; 
        rgbToRmt(red, green, blue, &led_data[index * 24]);
    }

    void WS2812_Simple::setPixelColor(int index, uint32_t color) {
        setPixelColor(index, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    }

    uint32_t WS2812_Simple::getPixelColor(int index) {
        if (index < 0 || index >= LED_COUNT) return 0;
        return pixelColors[index];
    }

    void WS2812_Simple::fill(uint8_t red, uint8_t green, uint8_t blue) {
        for (int i = 0; i < LED_COUNT; i++) {
            setPixelColor(i, red, green, blue);
        }
    }

    void WS2812_Simple::fillGradient(uint8_t startRed, uint8_t startGreen, uint8_t startBlue,
                                    uint8_t endRed, uint8_t endGreen, uint8_t endBlue) {
        for (int i = 0; i < LED_COUNT; i++) {
            float progress = (float)i / (LED_COUNT - 1);
            uint8_t red = startRed + progress * (endRed - startRed);
            uint8_t green = startGreen + progress * (endGreen - startGreen);
            uint8_t blue = startBlue + progress * (endBlue - startBlue);
            setPixelColor(i, red, green, blue);
        }
    }
    void WS2812_Simple::gradientMove(uint8_t startRed, uint8_t startGreen, uint8_t startBlue,
                                    uint8_t endRed, uint8_t endGreen, uint8_t endBlue) {
        static int gradientOffset = 0; 
        for (int i = 0; i < LED_COUNT; i++) {
            float progress = (float)((i + gradientOffset) % LED_COUNT) / LED_COUNT;
            uint8_t red = startRed + progress * (endRed - startRed);
            uint8_t green = startGreen + progress * (endGreen - startGreen);
            uint8_t blue = startBlue + progress * (endBlue - startBlue);
            setPixelColor(i, red, green, blue);
        }
        gradientOffset = (gradientOffset + 1) % LED_COUNT; 
    }

    void WS2812_Simple::setReverseOrder(bool reverse) {
        reverseOrder = reverse;
    }

    void WS2812_Simple::setColorOrder(ColorOrder order) {
        colorOrder = order;
    }

    void WS2812_Simple::applyColorOrder(uint8_t& red, uint8_t& green, uint8_t& blue) {
        if (colorOrder == COLOR_ORDER_RGB) {
            return; // RGB - default order
        } else if (colorOrder == COLOR_ORDER_GRB) {
            uint8_t temp = green;
            green = red;
            red = temp;
        }
    }

    void WS2812_Simple::rainbow() {
        for (int i = 0; i < LED_COUNT; i++) {
            int hue = (i * 360 / LED_COUNT) % 360;
            uint8_t red, green, blue;
            if (hue < 120) {
                red = (120 - hue) * 255 / 120;
                green = hue * 255 / 120;
                blue = 0;
            } else if (hue < 240) {
                red = 0;
                green = (240 - hue) * 255 / 120;
                blue = (hue - 120) * 255 / 120;
            } else {
                red = (hue - 240) * 255 / 120;
                green = 0;
                blue = (360 - hue) * 255 / 120;
            }
            setPixelColor(i, red, green, blue);
        }
    }
    void WS2812_Simple::rainbowMove() {
        static int hueOffset = 0; 
        for (int i = 0; i < LED_COUNT; i++) {
            int hue = (i * 360 / LED_COUNT + hueOffset) % 360; 
            uint8_t red, green, blue;
            if (hue < 120) {
                red = (120 - hue) * 255 / 120;
                green = hue * 255 / 120;
                blue = 0;
            } else if (hue < 240) {
                red = 0;
                green = (240 - hue) * 255 / 120;
                blue = (hue - 120) * 255 / 120;
            } else {
                red = (hue - 240) * 255 / 120;
                green = 0;
                blue = (360 - hue) * 255 / 120;
            }
            setPixelColor(i, red, green, blue);
        }
        hueOffset = (hueOffset + 1) % 360; // smooth motion
    }

    void WS2812_Simple::chase(uint8_t red, uint8_t green, uint8_t blue, int delayMs) {
        for (int i = 0; i < LED_COUNT; i++) {
            fill(0, 0, 0);
            setPixelColor(i, red, green, blue);
            show();
            vTaskDelay(delayMs / portTICK_PERIOD_MS);
        }
    }

    void WS2812_Simple::blink(uint8_t red, uint8_t green, uint8_t blue, int delayMs) {
        fill(red, green, blue);
        show();
        vTaskDelay(delayMs / portTICK_PERIOD_MS);
        fill(0, 0, 0);
        show();
        vTaskDelay(delayMs / portTICK_PERIOD_MS);
    }

    void WS2812_Simple::breathing(uint8_t red, uint8_t green, uint8_t blue, int durationMs) {
        for (int brightness = 0; brightness <= 255; brightness += 5) {
            setBrightness(brightness);
            fill(red, green, blue);
            show();
            vTaskDelay((durationMs / 2) / (255 / 5) / portTICK_PERIOD_MS);
        }
        for (int brightness = 255; brightness >= 0; brightness -= 5) {
            setBrightness(brightness);
            fill(red, green, blue);
            show();
            vTaskDelay((durationMs / 2) / (255 / 5) / portTICK_PERIOD_MS);
        }
    }

    void WS2812_Simple::show() {
        if (reverseOrder) {
            for (int i = 0; i < LED_COUNT / 2; i++) {
                uint32_t temp = pixelColors[i];
                pixelColors[i] = pixelColors[LED_COUNT - i - 1];
                pixelColors[LED_COUNT - i - 1] = temp;
            }
        }
        rmt_write_items(RMT_CHANNEL, led_data, LED_COUNT * 24, true);
        rmt_wait_tx_done(RMT_CHANNEL, portMAX_DELAY);
    }

    void WS2812_Simple::rgbToRmt(uint8_t red, uint8_t green, uint8_t blue, rmt_item32_t* data) {
        uint32_t rgb = (green << 16) | (red << 8) | blue;
        for (int i = 0; i < 24; i++) {
            data[i].duration0 = (rgb & (1 << (23 - i))) ? T1H : T0H;
            data[i].duration1 = (rgb & (1 << (23 - i))) ? T1L : T0L;
        }
    }
}