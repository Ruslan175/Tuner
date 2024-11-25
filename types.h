#ifndef  _TYPES_H_
#define  _TYPES_H_

#include <vector>
#include <cstdint>
#include <string>


struct CNoteData
{
    std::string     name;
    int             freq;

    CNoteData(const char *n, int f) : name(n), freq(f) {};
    CNoteData(std::string n, int f) : name(n), freq(f) {};
};
    
typedef     std::vector<CNoteData>          tNoteBuf;
typedef     int16_t                         tSoundSampleType;   // related to RTAUDIO_SINT16
typedef     std::vector<tSoundSampleType>   tSoundData;


#endif
