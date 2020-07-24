import time
import smbus
import sys
import subprocess
import os
import signal


bus = smbus.SMBus(1) #connect to IC2-bus
current = 1
proc = None

class C:
    def __init__(self, trigger, init_val=0):
        self._v = init_val
        self.trigger = trigger

    @property
    def v(self):
        return self._v

    @v.setter
    def v(self, value):
        global current
        global proc
        #If the value equals 1
        if value == self.trigger:
            #Make sure the process is not exist
            if (proc is None):
                #Start a background process
                print("Starting camera thread.")
                proc = subprocess.Popen("python3 /home/pi/project/server.py", shell=True, preexec_fn=os.setsid)
        else:
            #Make sure the process is not exist
            if not (proc is None):
                #Kill the process
                print("Closing camera thread.")
                os.killpg(proc.pid, signal.SIGTERM)
                proc = None
        self._v = value

#Create a class and set the trigger value as 1
c = C(1)


#Arduino address
address_arduino=0x8
print("Waiting for Argon's feedback...")

try:
    while True:
        try:
            #Read the I2C bus
            c.v=bus.read_byte(address_arduino)
            
            #Delay 2 sec
            time.sleep(2)
        except Exception:
            #When reading fails
            print("Argon is offline, program will look for it every 5 seconds.")
            
            #Delay 5 secs
            time.sleep(5)
            
            #Keep looping
            continue
except KeyboardInterrupt:
    pass






