from gpiozero import MotionSensor,LED
from signal import pause
pir = MotionSensor(18)
led = LED(15)

def motion_function():
    print("Motion Detected")
    led.on()

def no_motion_function():
    print("Motion stopped")
    led.off()


pir.when_motion = motion_function
pir.when_no_motion = no_motion_function

pause()
