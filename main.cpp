#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

#include "caser.hpp"

void SDCard::setSDState(eSDDetect state) {
    mInsertion = state;
}

bool SDCard::isInserted() const {
    return mInsertion == eSDDetect::Inserted;
}

// you are here
std::vector<SDRecord> getAllPlaylists(std::string dateTime) {
    std::vector<SDRecord> listRecords;
    //check insert, qryPlayList

    return listRecords;
}



int main() {

    SDCard sdCard;
    sdCard.setSDState(SDCard::eSDDetect::Inserted);

    return 0;
}