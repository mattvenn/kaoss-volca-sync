import os, sys, inspect, logging, time
import capablerobot_usbhub

hub = capablerobot_usbhub.USBHub()

# power cycle teensy
print("power cycle")
hub.power.disable([4])
time.sleep(0.5)
hub.power.enable([4])

