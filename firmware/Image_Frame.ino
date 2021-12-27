/*
Image frame from https://github.com/gaffo/inkplateImageFrame.

This is the main application class and it's job is to boot up on a timer,
sync configuration and images and then display whatever it should.
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE10
#error "Wrong board selection for this example, please select Inkplate 10 in the boards menu."
#endif

#include "Inkplate.h"
#include "auth.h" // keep your personal secrets in the auth file so you don't check them into source

Inkplate display(INKPLATE_3BIT);

// Store int in rtc data, to remain persistent during deep sleep
RTC_DATA_ATTR int bootCount = -1;

void setup()
{
    bootCount++;

    Serial.begin(115200);
    Serial.printf("Boot Number %d\n", bootCount);
    display.begin();

    const uint8_t *aryBytes[] = {
    };

    const int aryW[] = {
    };

    const int aryH[] = {
    };

    const char *aryDesc[] = {
    };

    const int len = 3;
    const uint64_t delay_seconds = 5;// 60 * 60;
    const int fullRefresh = 9;

    const int chosen = bootCount % len;
    Serial.printf("Using Index: %d\n", chosen);
    Serial.printf("Image: %s\n", aryDesc[chosen]);

    const uint8_t* bytes = aryBytes[chosen];
    const int w = aryW[chosen];
    const int h = aryH[chosen];

    bool result = display.drawImage(bytes, 0, 0, w, h);

    Serial.println(result);
    display.display();
    Serial.printf("Going to sleep for %d seconds or %d minutes or %d hours\n", delay_seconds, delay_seconds/60, delay_seconds/(60*60));

    esp_sleep_enable_timer_wakeup(delay_seconds * 1000 * 1000);
    esp_deep_sleep_start();
}

void loop()
{
    // Never here, as deepsleep restarts esp32
}