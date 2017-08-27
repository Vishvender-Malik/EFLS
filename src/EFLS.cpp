//============================================================================
// Name        : EFLS.cpp
// Author      : Dello
// Version     : 0.1 - Aug 2017
// Description : Emergency Forced Landing system
// Functions   :
//                    1. Chooses safe landing site based on Satellite images, processed in openCV
//                    2. Chooses safe landing site based on Open Street Maps, assisted by libOsmium
//                    3. Chooses safe landing site based on Terrain Data, uses SRTM data -- Yet to be implemented
//                    4. Chooses safe landing site based on Restricted areas, -- Yet to be implemented
//                    5. Selects landing site based on all above data, and within calculated UAV range.
//                    6. Landing site is rotated so UAV lands into the wind
//                    7. Landing path is planned based on Terrain data (yet to be done), and allows the UAV to do a correct approach
//                    8. Communication with UAV flight controller with ProtoBuf, only MavProxy binding available yet
//============================================================================

#include <DecisionEngine.h>

using namespace std;

int main() {
    DecisionEngine DE;
    DE.run();
    return 0;
}

