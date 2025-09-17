import RPi.GPIO as GPIO
from time import sleep

led_pin = 18

GPIO.setmode(GPIO.BCM)

GPIO.setup(led_pin, GPIO.OUT)

pwm_led = GPIO.PWM(led_pin , 100)

pwm_led.start(0)

for i in range(100):
	pwm_led.ChangeDutyCycle(i)
	sleep(0.1)
	print("Current duty cycle: {}\r".format(i))
