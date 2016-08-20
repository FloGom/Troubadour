# -*- coding: utf-8 -*-
"""
Created on Sat Jul 23 18:45:20 2016

@author: Florentin

script to change the volume in the raspi using buttons on GPIO
"""
# more info https://docs.python.org/2/library/subprocess.html
import subprocess


# On a Raspi B 256Mo
soundMin = -10239
soundMax = 400
# approximately 3%, 1% = 106.4

soundStep = 3

# get the current volume
output = subprocess.check_output("amixer cget numid=1", shell=True)

# prepare the value
currentVol = int(output.split(':')[1].split('|')[0].split('=')[1])
currentVolPercent = round((+currentVol-soundMin+1)/106.4)

newVol = currentVolPercent + soundStep
if(newVol>100):
    newVol = 100
elif(newVol<0):
    newVol = 0

#add 3% to the current volume
subprocess.call(['amixer', 'cset', 'numid=1', '{}%'.format(newVol)])
