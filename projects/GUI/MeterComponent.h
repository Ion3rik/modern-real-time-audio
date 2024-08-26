#pragma once

#include <JuceHeader.h>

#include "Meter.h"

namespace GUI
{

class MeterComponent : public juce::Component,
                       public juce::Timer
{
public:
    MeterComponent(DSP::Meter& meter);
    ~MeterComponent();

    static constexpr float MIN_DB_SCALE { -90.f };
    static constexpr float MAX_DB_SCALE { 0.f };

    void resized() override;
    void paint(juce::Graphics& g) override;
    void timerCallback() override;

private:
    DSP::Meter& meter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};

}
