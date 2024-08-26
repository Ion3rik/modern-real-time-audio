#pragma once

namespace DSP
{

class Oscillator
{
public:
    enum OscType : unsigned int
    {
        Sin = 0,
        TriAliased,
        SawAliased,
        TriAA,
        SawAA
    };

    Oscillator();
    ~Oscillator();

    Oscillator(const Oscillator&) = delete;
    Oscillator(Oscillator&&) = delete;
    const Oscillator& operator=(const Oscillator&) = delete;
    const Oscillator& operator=(Oscillator&&) = delete;

    // Update new sample rate
    void prepare(double sampleRate);

    // Process oscillator output for a buffer
    void process(float* output, unsigned int numSamples);

    // Process a single sample of the oscillator
    float process();

    // Set a new frequency for the oscillator in Hz
    void setFrequency(float freqHz);

    // Select the waveform type
    void setType(OscType type);

private:
    double sampleRate { 48000.0 };

    float frequency { 1.f };
    OscType type { Sin };

    float phaseState { 0.f };
    float phaseInc { 0.f };

    float differentiatorState { 0.f };
    float differentiatorCoeff { 0.f };

    // DPW methods

    float dpwTri();
    float dpwSaw();
};

}
