#!/usr/bin/env python

import pexpect, time, sys, os, csv

from MAVProxy.modules.mavproxy_efls import mavProxyLink_pb2

print "Turbo sim started"

protocolbuf_out = "/home/dello/EFLS/build/res/protobuf/aircraftLink_medium_aircraft"

os.system("mkdir /home/dello/EFLS/build/results/sites")

# Reads list of locations
with open('locations.csv', 'rb') as f:
    reader = csv.reader(f)
    csvList = list(reader)



for loop in range(0,40+1):
    print "Starting search number: " + str(loop)
    
    # Generate fake aircraft data
    # Defines aircarft data in Protobuf
    aircraft_link_out = mavProxyLink_pb2.AircraftLink()       
    aircraft = aircraft_link_out.aircrafts.add()
    aircraft.lat = float(csvList[loop][1])
    aircraft.lon = float(csvList[loop][2])
    aircraft.bearing = 0
    aircraft.speed = 20
    aircraft.altitude = 100
    aircraft.wind_speed = 10
    aircraft.wind_direction = 45
    aircraft.motor_current = 0
    aircraft.motor_throttle = 80       
    # Writes aircraft data to Protobuf file
    f = open(protocolbuf_out, "wb")
    f.write(aircraft_link_out.SerializeToString())
    f.flush()
    f.close
    
    # Start EFLS
    name = "python EFLS_start.py TEST" + str(loop)
    efls = pexpect.spawn(name, timeout=600)
    efls.expect("EFLS Finished")
    
    # Copy Image results for quicker viewing
    oldName = "/home/dello/EFLS/build/results/temp/landingSiteDisplayed.bmp"
    newName = "/home/dello/EFLS/build/results/sites/landingSite" + str(loop) + ".bmp"
    cmdName = "cp " + oldName + " " + newName
    os.system(cmdName)

    # Move folders to save results
    oldName = "/home/dello/EFLS/build/results/temp"
    newName = "/home/dello/EFLS/build/results/Test" + str(loop) 
    cmdName = "mv " + oldName + " " + newName
    cmdNameMk = "mkdir " + oldName
    os.system(cmdName)
    os.system(cmdNameMk)

print "Turbo sim completed"
    
    
    
    



