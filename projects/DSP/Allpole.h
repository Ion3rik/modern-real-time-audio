#pragma once

#include <array>
#include <vector>

namespace DSP
{

class Allpole
{
public:
    Allpole(unsigned int filterOrder, unsigned int maxNumChannels);
    Allpole();
    ~Allpole();

    Allpole(const Allpole&);
    const Allpole& operator=(const Allpole&);

    Allpole(Allpole&&) = delete;
    const Allpole& operator=(Allpole&&) = delete;

    // Clear all states
    void clear();

    // Reallocate state storage
    // Calling this method will clear the states
    void reallocateChannels(unsigned int maxNumChannels);

    // Set new coeffs
    void setCoeffs(const std::vector<float>& newCoeffs);

    // Process audio
    // This method can be called with a lower number of channels than allocated
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

    // Process audio
    // Single sample flavour
    void process(float* output, const float* input, unsigned int numChannels);

    // return the number of currently allocated channels
    unsigned int getAllocatedChannels() const noexcept { return allocatedChannels; }

private:
    unsigned int allocatedChannels { 0 };
    unsigned int filterOrder { 0 };

    // vector of coeffs
    // [b0, a1, a2, a3... aN]
    std::vector<float> coeffs;

    // vector of states of all channels and sections
    // [ch0_sos0_bz1, ... , ch0_sos0_az2, ch0_sos1_bz1, ... , ch0_sos1_az2, ... ,
    //  ch1_sos0_bz1, ... , ch1_sos0_az2, ch1_sos1_bz1, ... , ch1_sos1_az2, ...]
    std::vector<float> states;
};

}