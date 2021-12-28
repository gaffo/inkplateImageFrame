#include <ArduinoJson.h>

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
#include "sd_image_repo.h"
#include "error_image.h"

#define uS_TO_S_FACTOR 1000000UL

Inkplate display(INKPLATE_3BIT);

// Store int in rtc data, to remain persistent during deep sleep
RTC_DATA_ATTR int bootCount = -1;

void setup()
{
    bootCount++;

    Serial.begin(115200);
    Serial.printf("Boot Number %d\n", bootCount);
    display.begin();

    // Init the SD Card
    Serial.println("Initting SD Card");

    if (false) {
        error("TEST ERROR");
        return;
    }

    if (!display.sdCardInit()) {
        Serial.println("Unable to init sd card");
        error("SD CARD INIT");
        return;
    }

    Serial.println("Starting SDRepo");

    SDRepo sdr;
    if (!sdr.init()) {
        Serial.println("Error initting SDRepo, aborting");
        error("SD REPO INIT");
        return;
    }

    // TODO: Sync from internet

    // Fetch next image
    if (!sdr.setIndex(bootCount)) {
        Serial.println("Error setting current image, aborting");
        error("SETTING CURRENT IMAGE");
        return;
    }

    const char* currentFile = sdr.currentFile();
    if (currentFile == NULL) {
        Serial.println("Got back null current image, aborting");
        error("NULL FILE");
        return;
    }

    // Try and display the image
    Serial.printf("Using image name: %s\n", currentFile);
    if (!display.drawImage(currentFile, 0, 0, 1)) {
        Serial.println("Unable to draw requested image");
        error(currentFile);
        return;
    }

    // Ok time to sleep`  
    sleep(sdr.sleepSeconds());
}

void loop()
{
    // Never here, as deepsleep restarts esp32
}

void error(const char* errorReason) {
    Serial.println("Displaying error image");

    display.drawImage(error_image, 0, 0, error_image_w, error_image_h);

    display.setTextWrap(true);
    display.setTextColor(0, 7);
    display.setCursor(0, 0);
    display.setTextSize(7);
    display.printf("ERROR: %s", errorReason);

    sleep(60);
}

void sleep(int sleepSeconds) {
    // Flush Display
    display.display();

    // Sleep
    Serial.printf("Going to sleep for %d seconds.\n", sleepSeconds);
    esp_sleep_enable_timer_wakeup(sleepSeconds * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}