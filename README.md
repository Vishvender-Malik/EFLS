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


# Use of EFLS:
While being a standalone package designed to be used on a onboard computer system on the UAV, it does require a binding method into the UAV flight controller. This has been done through the use of Google's Protocol Buffer 3 (proto3) allowing for a  general method of comunication with EFLS and the flight controller. For use with any flight controller that supports providing (location, altitude, speed, direction, wind speed, wind direction, and trigger message) and setting of waypoints for autonomous landing. 

The only official support currently availiable is for the PixHawk flight controller, running ArduPlane, with proto3 connecting to a MAVProxy module. While MAVProxy is a GCS, it is used in this case to only convert MAVLink messages into proto3 messages. 

## Install of EFLS:
The following are the required steps to get EFLS installed and working:
1. Install the latest MAVProxy (currently need to use dell-o/MAVProxy/)
2. Compile EFLS, refer below for compile instructions.
3. The compiled EFLS script should be located in the modules/MAVProxy_EFLS/ folder. (ToDo - Need to change paths to point to this location)
4. Test setup with SITL

### Compile requirements for EFLS
The following are the required steps to compile EFLS:
1. Download src/ from this respositiory.
2. Install openCV
3. Install libosmium
4. Install curl
5. Install proto3 - only required if changing the protocol buffer
6. Compile using C++ compiler (minimum C++11 is required) - CMake file must be used


