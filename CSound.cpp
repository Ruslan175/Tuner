#include <cmath>
#include "CSound.hpp"


CSound::CSound(unsigned int rate)
    : mDataTimeStep(1.0/static_cast<double>(rate))
    , mMaxFrequency(18000.0)
{
}

void CSound::setUpFrequencyLimit(double f)
{
    mMaxFrequency = f;
}

int CSound::getTopFrequency(const tSoundData &data)
{
    return useFFT(data); // useDFT(data);
}

// DFT: https://www.sanfoundry.com/cpp-program-compute-discrete-fourier-transform-using-naive-approach/
// https://en.wikipedia.org/wiki/Discrete_Fourier_transform
double CSound::getAmpDFT(const tSoundData &data, int k)
{
    const unsigned int sz = data.size();
    const double r = (2.0 * k * M_PI) / sz;
    std::vector<double> cosine(sz);
    std::vector<double> sine(sz);

    for (unsigned int i = 0u; i < sz; ++i)
    {
        cosine[i] = cos(i * r);
        sine[i] = sin(i * r);
    }

    double real{0.0};
    double img{0.0};

    for (unsigned int i = 0u; i < sz; ++i)
    {
        real += data[i] * cosine[i];
        img  += data[i] * sine[i];
    }

    return sqrt(real * real + img * img);
}

int CSound::useDFT(const tSoundData &data)
{
    int max_freq{0};
    const double r = 1.0 / (mDataTimeStep * data.size());  
    double amp_max{0.0};
 
    for (unsigned int k = 0u; k < data.size(); ++k)
    {
        double freq = k * r;
        if (freq >= mMaxFrequency) break; // too high frequency
        double amp = getAmpDFT(data, k);
        if (amp > amp_max)
        {
            amp_max = amp;
            max_freq = static_cast<int>(freq);
        }
    }
    return max_freq;
}


// https://tfetimes.com/c-fast-fourier-transform/
// Cooley–Tukey FFT (in-place)
void CSound::CooleyTukeyFFT(CArray& x)
{
	const size_t N = x.size();
	if (N <= 1) return;

	// divide
	CArray even = x[std::slice(0, N/2, 2)];
	CArray  odd = x[std::slice(1, N/2, 2)];

	// conquer
	CooleyTukeyFFT(even);
	CooleyTukeyFFT(odd);

	// combine
    const double r = -2.0 * M_PI / N;
	for (size_t k = 0; k < N/2; ++k)
	{
		tComplex t = std::polar(1.0, r * k) * odd[k];
		x[k    ] = even[k] + t;
		x[k + N/2] = even[k] - t;
	}
}

int CSound::useFFT(const tSoundData &data)
{
    // Prepare and run FFT
    CArray ar(data.size());
    int sz = 0;
    for (auto i : data) ar[sz++] = tComplex(static_cast<double>(i), 0.0);
    CooleyTukeyFFT(ar);

    // Looking for frequency of max magnitude
    const double r = 1.0 / (mDataTimeStep * data.size());
    double max_mag = std::abs(ar[0]);
    int max_freq{0};
    double freq{0.0};
    for (unsigned int i = 1u; i < ar.size(); ++i)
    {
        freq += r;
        if (freq >= mMaxFrequency)  break; // too high frequency
        double mag = std::abs(ar[i]);
        if (mag > max_mag)
        {
            max_mag = mag;
            max_freq = static_cast<unsigned int>(freq);
        }
    }
    return max_freq;
}

