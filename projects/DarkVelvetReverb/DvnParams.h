# pragma once

#include <vector>
#include "JuceHeader.h"

namespace Params
{
    enum RirModel : unsigned int
{
    Room1 = 0,
    Room2,
    Room3
};

struct DvnParams
{
    DvnParams();

    void switchRoom(Params::RirModel newRirModel);

    unsigned int numFilter;
    unsigned int numPulse;
    unsigned int dictionaryFilterOrder;
    unsigned int postFilterOrder;
    std::vector<unsigned int> numPulsePerChannel;
    unsigned int numChannel;
    Params::RirModel currentModel = Room1; // default to Room0
    std::vector<unsigned int> filterRouting;
    std::vector<std::vector<unsigned int>>  pulseLocation;
    std::vector<std::vector<float>>  pulseGain;
    std::vector<float> channelGain;
    std::vector<std::vector<float>> dictionaryFilterCoeff;
    std::vector<float> postFilterCoeff;

    private: 
        void populateParams(RirModel newRirModel);
        void splitPulsesToFilters(std::vector<std::vector<unsigned int>> &globalPulseLocation, std::vector<std::vector<float>> &globalPulseGain);


};  
}