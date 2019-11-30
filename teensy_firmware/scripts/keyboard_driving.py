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

ser = serial.Serial("/dev/ttyACM1",115200)
ser.flushInput()


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
    char = ord(getch())

    if char == W_KEY:
    	ser.write("W")
	elif char == A_KEY:
		ser.write("A")
	elif char == S_KEY:
		ser.write("S")
	elif char == D_KEY:
		ser.write("D")
