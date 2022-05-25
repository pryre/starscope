from machine import Pin
import time
import sys, os

from src.starscope import Starscope

starscope = Starscope()

pin = Pin(25, Pin.OUT)

pin.toggle()
# starscope.screen.run_demo()

try:
    starscope.begin()
    # starscope.screen.run_demo()

    while True:
        pin.toggle()
        time.sleep_ms(500)
except Exception as e:
    raise e
finally:
    starscope.shutdown()