#include <iostream>
#include <cstring>
#include "CMicrophone.hpp"

#define    _LOCK_GUARD  std::lock_guard<std::mutex> lock(CMicrophone::sMtx);


bool CMicrophone::sReady = !CMicrophone::getObject().mBuffer.empty(); // just to init object
std::mutex   CMicrophone::sMtx;

CMicrophone::CMicrophone()
{
    do
    {
        // Check audio device number
        std::vector<unsigned int> deviceIds = mAudio.getDeviceIds();
        if (0u == deviceIds.size()) 
        {
            std::cout << "CMicrophone(): No audio devices found!"  << std::endl;
            break;
        }
        
        // Open microphone stream
        RtAudio::StreamParameters param;
        param.deviceId = mAudio.getDefaultInputDevice();
        param.nChannels = 1;
        param.firstChannel = 0;
        if (RTAUDIO_NO_ERROR != mAudio.openStream(NULL, &param, RTAUDIO_SINT16, mRate, &mBufFrames, &CMicrophone::cbkMicro)) 
        {
            std::cout << "CMicrophone(): openStream error: " << mAudio.getErrorText() << std::endl;
            break;
        }

        // Start audio stream
        if (RTAUDIO_NO_ERROR != mAudio.startStream()) 
        {
            std::cout << "CMicrophone(): startStream error: " << mAudio.getErrorText() << std::endl;
            break;
        }
        mBuffer.resize(mBufFrames, 0);
    }
    while(false);
}

CMicrophone::~CMicrophone()
{
    if (true == mAudio.isStreamRunning())   mAudio.stopStream();
    if (true == mAudio.isStreamOpen())      mAudio.closeStream();
}

CMicrophone& CMicrophone::getObject()
{
    static CMicrophone micro = CMicrophone();
    return micro;
}

bool CMicrophone::isReady()
{
    return sReady;
}

unsigned int CMicrophone::getDataRate()
{
    return getObject().mRate;
}

int CMicrophone::cbkMicro(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                           double streamTime, RtAudioStreamStatus status, void *userData)
{
    _LOCK_GUARD
    CMicrophone &obj = getObject();
    obj.mBuffer.resize(nBufferFrames);
    memcpy(obj.mBuffer.data(), inputBuffer, nBufferFrames * sizeof(tSoundSampleType));
    return 0;
}

void CMicrophone::getSound(tSoundData &data)
{
    _LOCK_GUARD
    data = getObject().mBuffer;
}

