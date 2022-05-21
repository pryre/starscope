from machine import Pin
import time

from src.starscope import Starscope

starscope = None
try:
    starscope = Starscope()

    pin = Pin(25, Pin.OUT)

    while True:
        pin.toggle()
        time.sleep_ms(500)
except:
    pass
finally:
    if starscope:
        starscope.shutdown()