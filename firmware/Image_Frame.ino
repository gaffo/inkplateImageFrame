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

    if (!display.sdCardInit()) {
        Serial.println("Unable to init sd card");
        return;
    }

    Serial.println("Starting SDRepo");

    SDRepo sdr;
    if (!sdr.init()) {
        Serial.println("Error initting SDRepo, aborting");
        error(sdr);
        return;
    }

    // TODO: Sync from internet

    // Fetch next image
    if (!sdr.setIndex(bootCount)) {
        Serial.println("Error setting current image, aborting");
        error(sdr);
        return;
    }

    const char* currentFile = sdr.currentFile();
    if (currentFile == NULL) {
        Serial.println("Got back null current image, aborting");
        error(sdr);
        return;
    }

    // Try and display the image
    Serial.printf("Using image name: %s\n", currentFile);
    if (!display.drawImage(currentFile, 0, 0, 1)) {
        Serial.println("Unable to draw requested image");
        error(sdr);
        return;
    }

    // Ok time to sleep`        
    sleep(sdr);
}

void loop()
{
    // Never here, as deepsleep restarts esp32
}

void error(SDRepo &sdr) {
    Serial.println("Displaying error image");

    // TODO: Display error image

    sleep(sdr);
}

void sleep(SDRepo &sdr) {
    display.display();
    int sleepSeconds = sdr.sleepSeconds();
    Serial.printf("Going to sleep for %d seconds.\n", sleepSeconds);

    esp_sleep_enable_timer_wakeup(sleepSeconds * 1000 * 1000);
    esp_deep_sleep_start();
}