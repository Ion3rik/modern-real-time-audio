#pragma once

#include <JuceHeader.h>

namespace GUI
{

class MrtaLAF : public juce::LookAndFeel_V4
{
public:
    MrtaLAF();
    ~MrtaLAF();

    void drawRotarySlider(Graphics&, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                          Slider&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MrtaLAF)
};

}
