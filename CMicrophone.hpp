#ifndef  _CMICROPHONE_HPP
#define  _CMICROPHONE_HPP

#include <mutex>
#include "RtAudio.h"
#include "types.h"



class CMicrophone
{ // One microphone - one way to get it (i.e. singelton)
    public:
    static void getSound(tSoundData &data); // fills up the buffer by microphone data
    static bool isReady();                  // checks if a microphone can be reached
    static unsigned int getDataRate();      // returns how many sound samples are obtained per second


    private:
    static std::mutex   sMtx;
    static bool         sReady;
    RtAudio mAudio;
    const unsigned int  mRate{40000};
    unsigned int        mBufFrames{512}; // size should be 2^n due to DFT/FFT
    tSoundData          mBuffer;


    static int cbkMicro( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                        double streamTime, RtAudioStreamStatus status, void *userData);

    static CMicrophone& getObject();

    CMicrophone();
    ~CMicrophone();
    CMicrophone(const CMicrophone&) = delete;
    CMicrophone(CMicrophone&&) = default;
    CMicrophone& operator=(CMicrophone&) = delete;
    CMicrophone& operator=(CMicrophone&&) = delete;
};

#endif
