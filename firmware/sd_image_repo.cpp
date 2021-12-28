#include "sd_image_repo.h"

#include "SdFat.h"

#define KEY_IMAGES "images"

SDRepo::SDRepo()
: m_jdoc(4086),
  m_index(0), 
  m_count(0), 
  m_sleepSeconds(0) {

}

bool SDRepo::init() {
    Serial.println("SDRepo::init()");
    SdFile file;

    Serial.println("Opening Config");
    if (!file.open("/config.json", O_RDONLY)) {
        Serial.println("Error opening /config.json");
        return false;
    }

    Serial.println("deserializeJson");
    DeserializationError error = deserializeJson(m_jdoc, file);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    Serial.println("Getting sleepSeconds");
    m_sleepSeconds = m_jdoc["sleepSeconds"];

    Serial.println("Getting count of images");
    m_count = m_jdoc[KEY_IMAGES].size();

    Serial.println("SDRepo::init() returning success");
    return true;
}

bool SDRepo::setIndex(int count) {
    m_index = count % m_count;
}

const char* SDRepo::currentFile() const {
    return m_jdoc[KEY_IMAGES][m_index];
}