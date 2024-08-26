#include "MrtaLAF.h"

namespace GUI
{

MrtaLAF::MrtaLAF()
{
}

MrtaLAF::~MrtaLAF()
{
}

void MrtaLAF::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                               float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                               Slider&)
{
    // total area of out slider
    juce::Rectangle<float> area(x, y, width, height);

    // proportional rect of slider position
    juce::Rectangle<float> fillProp(0.25f, 1.f - sliderPosProportional,
                                    0.5f, sliderPosProportional);

    // proportional area of outline
    juce::Rectangle<float> outlineProp(0.25f, 0.f, 0.5f, 1.f);

    // fill slider position rect
    g.setColour(juce::Colours::grey);
    g.fillRoundedRectangle(area.getProportion(fillProp), 5.f);

    // draw slider outline rect
    g.setColour(juce::Colours::darkgrey);
    g.drawRoundedRectangle(area.getProportion(outlineProp),
                           5.f, 4.f);
}

}
