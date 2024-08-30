# pragma once

#include <vector>

namespace Params
{
    enum RirModel : unsigned int
{
    Room0 = 0,
    Room1,
    Room2
};

struct DvnParams
{
    DvnParams();

    void switchRoom(Params::RirModel newRirModel);

   /* unsigned int filterRouting[NUM_PULSE];
    unsigned int pulseLocation[NUM_PULSE * NUM_CHANNEL];
    float pulseGain[NUM_PULSE * NUM_CHANNEL];
    float channelGain[NUM_CHANNEL];
    float dictionaryFilterCoeff[(D_FILTER_ORDER + 1) * NUM_FILTER];
    float DictionaryFilterCoeff[(D_FILTER_ORDER + 1) * NUM_FILTER];
    float PostFilterCoeff[P_FILTER_ORDER + 1];*/

    unsigned int numFilters;
    unsigned int numPulses;
    RirModel currentModel = Room0; // default to Room0
    std::vector<unsigned int> filterRouting;
    std::vector<unsigned int> pulseLocation;
    std::vector<float> pulseGain;
    std::vector<float> channelGain;
    std::vector<float> dictionaryFilterCoeff;
    std::vector<float> postFilterCoeff;

    private: 
        void populateParams(RirModel newRirModel);

};  
}