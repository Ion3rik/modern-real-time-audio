#include "DvnParams.h"

namespace Params 
{
DvnParams::DvnParams()
{
     // default to Room0
    currentModel = Room0;
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
            channelGain = {1, 1};
        case 1: // ROOM 2
            channelGain = {1, 1};
        case 2: // ROOM 3
            channelGain = {1, 1};
    }

}
}