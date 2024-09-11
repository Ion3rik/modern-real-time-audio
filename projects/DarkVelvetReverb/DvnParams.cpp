#include "DvnParams.h"
#include "Room1.h"

namespace Params 
{
DvnParams::DvnParams()
{
     // default to Room0
    currentModel = Room1;
    populateParams(currentModel);
}

void DvnParams::switchRoom(RirModel newRirModel)
{
    populateParams(newRirModel); // call function that populates the member variables
}


// Function to populate the member variables with the desired data
void DvnParams::populateParams(RirModel newRirModel)
{    
    switch(currentModel)
    {
        case 0: // ROOM 1
        {
            Params::Room1Params* params = nullptr;
            params = params = new Params::Room1Params(); 
            numFilter = params->numFilter;
            numPulse = params->numPulse;
            numChannel = params->numChannel;
            filterRouting = params->filterRouting;
            splitPulsesToFilters(params->pulseLocation, params->pulseGain);
            channelGain = params->channelGain;
            dictionaryFilterCoeff = params->dictionaryFilterCoeff;
            postFilterCoeff = params->postFilterCoeff;
            dictionaryFilterOrder = dictionaryFilterCoeff[0].size()-1;
            postFilterOrder = postFilterCoeff.size()-1;
            delete params;
            break;
        }
        case 1: // ROOM 2
        {
            Params::Room1Params* params = nullptr;
            params = params = new Params::Room1Params(); 
            numFilter = params->numFilter;
            numPulse = params->numPulse;
            numChannel = params->numChannel;
            filterRouting = params->filterRouting;
            splitPulsesToFilters(params->pulseLocation, params->pulseGain);
            channelGain = params->channelGain;
            dictionaryFilterCoeff = params->dictionaryFilterCoeff;
            postFilterCoeff = params->postFilterCoeff;
            delete params;
            break;
        }
        case 2: // ROOM 3
        {
            Params::Room1Params* params = nullptr;
            params = params = new Params::Room1Params(); 
            numFilter = params->numFilter;
            numPulse = params->numPulse;
            numChannel = params->numChannel;
            filterRouting = params->filterRouting;
            splitPulsesToFilters(params->pulseLocation, params->pulseGain);
            channelGain = params->channelGain;
            dictionaryFilterCoeff = params->dictionaryFilterCoeff;
            postFilterCoeff = params->postFilterCoeff;
            delete params;
            break;
        }
    }

}

void DvnParams::splitPulsesToFilters(std::vector<std::vector<unsigned int>> &globalPulseLocation, std::vector<std::vector<float>> &globalPulseGain)
{
    // Init the vectors 
    pulseLocation.clear();
    pulseGain.clear();
    numPulsePerChannel.clear();
    for (unsigned int k = 0; k < numChannel*numFilter; ++k)
    {
        pulseLocation.push_back(std::vector<unsigned int>());
        pulseGain.push_back(std::vector<float>());
        numPulsePerChannel.push_back(0);
    }
    
    for (unsigned int ch = 0; ch < 2; ++ch)
    {
        unsigned int channelOffset = ch*numFilter;
        for (unsigned int q = 0; q < numFilter; ++q)
        {
            for(unsigned int m = 0; m < numPulse; ++m)
            {
                if (filterRouting[m] == q)
                {
                    pulseLocation[q+channelOffset].push_back(globalPulseLocation[ch][m]);
                    pulseGain[q+channelOffset].push_back(globalPulseGain[ch][m]);
                }
            }
            numPulsePerChannel[q] = pulseLocation[q].size(); // save the number of pulses for current filter
            numPulsePerChannel[q+numFilter] = pulseLocation[q+numFilter].size();
        }
     }
}
}