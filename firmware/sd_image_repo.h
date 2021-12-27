#pragma once

typedef char filename[256];

class SDRepo {
public:
    // Call init before using, check error code otherwise
    bool init();

    // Set the current image index, internally we will modulo
    // off of the total number of images
    bool setIndex(int count);

    // returns a pointer to the current filename
    // we do not need to gc this, as it's just a pointer
    // to internal data
    const filename* currentFile();
};