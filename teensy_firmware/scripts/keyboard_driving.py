import serial
import sys
import termios
import tty
import sys
import time

W_KEY = 119
A_KEY = 97
S_KEY = 115
D_KEY = 100

# loop throttle to jive with microcontroller (us)
throttle_speed = 0.0005

# open serial connection to teensy
ser = serial.Serial("/dev/ttyACM0",115200)
ser.flushInput()

# function to get input from keyboard
def getch():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)
 
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch
 
button_delay = 0.2
 
while True:

    start_time = time.time()

    char = ord(getch())
    print(char)
    if char == W_KEY:
    	ser.write("W")
    elif char == A_KEY:
		ser.write("A")
    elif char == S_KEY:
		ser.write("S")
    elif char == D_KEY:
        ser.write("D")

    if char == 3:
        break

    elapsed_time = time.time() - start_time
    if elapsed_time < throttle_speed:
        time.sleep(throttle_speed - elasped_time)


