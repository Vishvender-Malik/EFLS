# EFLS - Emergency Forced Landing System for UAVs

EFLS is a standalone C++ software package that allows UAVs to find a suitable landing site in an emergency scenario. EFLS utilises the following information sources to find a suitable landing site:
* Google satellite images, accessed using curl, and processed by OpenCV
* OSM street maps, accessed using libosmium 
* SRTM Terrain data, Not completed
* Custom exclusion and inclusion regions, Not completed

The goal of EFLS is to allow a UAV to land without hurting people and damaging property. The full strategy is broken down into 3 phases. Each phase has weaknesses in achieving the goal however when all 3 phases operate consecutively a robust solution is achieved. The software currently produced is only based on the phase 1 approach, future work will be aimed at improving phase 1 and some phase 2 support.
1. Geospatial analysis using satellite image, street maps, and terrain data.  
    Benefits: Unaffected by weather conditions, time of day, and visual range to site.    
    Weaknesses: Affected by age of information used and dynamic elements (e.g. People walking across landing site).

2. Live image recognition using onboard cameras.  
    Benefits: Current information of landing site.  
    Weaknesses: Affected by weather conditions, time of day, and visual range to site.
    
3. Collision detection and avoidance during final approach and landing.   
    Benefits: Protects against crashing into dynamic moving objects or building/trees not seen from the overhead scanning.  
    Weaknesses: Affected by weather conditions and time of day.

# EFLS Architecture 
![alt text](https://github.com/dell-o/EFLS/blob/master/Code%20module%20breakdown%20v2.bmp "EFLS Architecture")



