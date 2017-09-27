#!/usr/bin/env python

import pexpect, time, sys, os
os.chdir("/home/dello/workspace/EFLS/build")
efls = pexpect.spawn("./EFLS", logfile=sys.stdout, timeout = 600)
efls.expect("DecisionEngine: Closing")
efls.close(force=True)


