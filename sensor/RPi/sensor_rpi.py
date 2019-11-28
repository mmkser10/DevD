import RPi.GPIO as GPIO
import time

s2 = 25
s3 = 26
signal = 27
NUM_CYCLES = 10


def setup():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(signal,GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(s2,GPIO.OUT)
    GPIO.setup(s3,GPIO.OUT)
    print("GPIO setting over.\n")


def loop():
    temp = 1
    while(1):
        print("[do loop]\n")
        GPIO.output(s2,GPIO.LOW)
        GPIO.output(s3,GPIO.LOW)
        time.sleep(0.3)
        print("[edge detection(red) start]\n")
        start = time.time()
        for impulse_count in range(NUM_CYCLES):
            GPIO.wait_for_edge(signal, GPIO.FALLING)
        duration = time.time() - start 
        red  = NUM_CYCLES / duration
        print("[red detection over]\n")

        GPIO.output(s2,GPIO.LOW)
        GPIO.output(s3,GPIO.HIGH)
        time.sleep(0.3)
        print("[edge detection(blue) start]\n")
        start = time.time()
        for impulse_count in range(NUM_CYCLES):
            GPIO.wait_for_edge(signal, GPIO.FALLING)
        duration = time.time() - start
        blue = NUM_CYCLES / duration
        print("[blue detection over]\n")
    
        GPIO.output(s2,GPIO.HIGH)
        GPIO.output(s3,GPIO.HIGH)
        time.sleep(0.3)
        print("[edge detection(green) start]\n")
        start = time.time()
        for impulse_count in range(NUM_CYCLES):
            GPIO.wait_for_edge(signal, GPIO.FALLING)
        duration = time.time() - start
        green = NUM_CYCLES / duration
        print("[green detection over]\n")


        if green<7000 and blue<7000 and red>12000:
            print("- result = red\n")
            temp=1
        elif red<12000 and  blue<12000 and green>12000:
            print("- result = green\n")
            temp=1
        elif green<7000 and red<7000 and blue>12000:
            print("- result = blue!\n")
            temp=1
        elif red>10000 and green>10000 and blue>10000 and temp==1:
            print("- place the object.....\n")
            temp=0

def endprogram():
    GPIO.cleanup()


if __name__=='__main__':
   
    setup()

    try:
        loop()

    except KeyboardInterrupt:
        endprogram()
