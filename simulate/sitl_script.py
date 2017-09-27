#!/usr/bin/env python

import pexpect, time, sys, os
from pymavlink import mavutil

def wait_heartbeat(mav, timeout=10):
    '''wait for a heartbeat'''
    start_time = time.time()
    while time.time() < start_time+timeout:
        if mav.recv_match(type='HEARTBEAT', blocking=True, timeout=0.5) is not None:
            return
    failure("Failed to get heartbeat")    

def wait_mode(mav, modes, timeout=10):
    '''wait for one of a set of flight modes'''
    start_time = time.time()
    last_mode = None
    while time.time() < start_time+timeout:
        wait_heartbeat(mav, timeout=10)
        if mav.flightmode != last_mode:
            print("Flightmode %s" % mav.flightmode)
            last_mode = mav.flightmode
        if mav.flightmode in modes:
            return
    print("Failed to get mode from %s" % modes)
    sys.exit(1)

def wait_time(mav, simtime):
    '''wait for simulation time to pass'''
    imu = mav.recv_match(type='RAW_IMU', blocking=True)
    t1 = imu.time_usec*1.0e-6
    while True:
        imu = mav.recv_match(type='RAW_IMU', blocking=True)
        t2 = imu.time_usec*1.0e-6
        if t2 - t1 > simtime:
            break

for loop in range(0,20):
    cmd = 'sim_vehicle.py -S 5 -L Test' + str(loop) +' --map --aircraft test -v ArduPlane'
    
    mavproxy = pexpect.spawn(cmd, logfile=sys.stdout, timeout=600)
    mavproxy.expect("online")
    
    mav = mavutil.mavlink_connection('127.0.0.1:14550')
    
    print("SITL_SCRIPT: Staring SITL")
    
    mavproxy.send('module load map\n')
    mavproxy.expect("EKF2 IMU1 Origin set to GPS")
    mavproxy.expect("EKF2 IMU0 is using GPS")
    mavproxy.send('wp load defaultWaypoints\n')
    wait_time(mav, 10)
    mavproxy.send('mode AUTO\n')
    wait_time(mav, 10)
    mavproxy.send('wp list\n')
    wait_time(mav, 10)
    mavproxy.send('wp movemulti 1 1 13 0 0\n')
    wait_time(mav, 10)
    mavproxy.send('module load efls\n')
    wait_time(mav, 10)
    mavproxy.send('arm safetyoff\n')
    mavproxy.send('arm throttle\n')
    mavproxy.expect("ARMED")
    wait_time(mav, 20)
    efls = pexpect.spawn("python EFLS_start.py", timeout=600)
    
    mavproxy.expect("Auto disarmed")
    wait_time(mav, 10)
    #mavproxy.logfile = None
    #mavproxy.interact()
    print("Closing SITL")
    mavproxy.close(force=True)
    
    oldName = "/home/dello/workspace/EFLS/build/results/temp"
    newName = "/home/dello/workspace/EFLS/build/results/Test" + str(loop) 
    cmdName = "mv " + oldName + " " + newName
    cmdNameMk = "mkdir " + oldName
    os.system(cmdName)
    os.system(cmdNameMk)


