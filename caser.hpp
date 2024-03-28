#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>


struct SDRecord {

};


class SDCard {
public:
    enum class eSDDetect {
        Removed,
        Inserted
    };

    SDCard() : mInsertion(eSDDetect::Removed) {}
    
    // declare function here
    void setSDState(eSDDetect state);
    bool isInserted() const;
    std::vector<SDRecord> getAllPlaylists(std::string dateTime);

private:
    eSDDetect mInsertion;



};


class SDPlayback {
    
};