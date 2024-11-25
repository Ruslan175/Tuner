#ifndef  _CSOUND_HPP
#define  _CSOUND_HPP

#include <complex>
#include <valarray>
#include "types.h"


class CSound
{
    public:
    CSound(unsigned int rate);
    int getTopFrequency(const tSoundData &data);   // returns in Hz
    void setUpFrequencyLimit(double f);            // to cut off unused frequencies

    private:
    typedef     std::complex<double>    tComplex;
    typedef     std::valarray<tComplex> CArray;   

    const double mDataTimeStep;
    double mMaxFrequency;
    
    int useFFT(const tSoundData &data);
    void CooleyTukeyFFT(CArray& x);

    int useDFT(const tSoundData &data);
    double getAmpDFT(const tSoundData &data, int k);
};

#endif
