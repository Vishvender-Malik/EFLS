#!/usr/bin/env python

import pexpect, time, sys, os
os.chdir("/home/dello/EFLS/build")
name = "./EFLS " + str(sys.argv[1])
efls = pexpect.spawn(name)
efls.expect("DecisionEngine: Closing")
efls.close(force=True)
print "EFLS Finished"


