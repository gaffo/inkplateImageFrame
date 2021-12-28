#pragma once

#include <ArduinoJson.h>

class SDRepo {
public:
    SDRepo();

    // Call init before using, check error code otherwise
    bool init();

    // Set the current image index, internally we will modulo
    // off of the total number of images
    bool setIndex(int count);

    // returns a pointer to the current filename
    // we do not need to gc this, as it's just a pointer
    // to internal data
    const char* currentFile() const;

    // Returns the number of seconds to sleep
    int sleepSeconds() const {
        return m_sleepSeconds;
    }

private:
    DynamicJsonDocument m_jdoc;
    int m_index;
    int m_count;

    // Cached values
    int m_sleepSeconds;
};