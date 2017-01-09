#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc, char* argv[]) {

	// Serial port structure for configuration
	struct termios serial;
	// Ports are defined like files in Unix
	char* port = "/dev/ttyS0";
	// Stores input key from keyboard
	char  key;

	// Print which port is going to be opened to console
	printf("Opening %s\n", port);

	// Open port
	int uartPort = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

	// Check to make sure port was opened
	if(uartPort == -1) {
		perror("Failed to open port");
		return -1;
	}

	// Configure UART port
	// See http://man7.org/linux/man-pages/man3/termios.3.html
	//  for details on below configuration parameters
	serial.c_lflag = 0;		// Input modes
	serial.c_oflag = 0;		// Output modes
	serial.c_iflag = 0;		// Control modes
	serial.c_cflag = 0;		// Local modes
	serial.c_cc[VMIN] = 0;	// Special characters
	serial.c_cc[VTIME] = 0;	// Special characters
	serial.c_cflag = B115200 | CS8 | CREAD;

	// Apply configuration
	tcsetattr(uartPort, TCSANOW, &serial);

	// Run forever
	while(1) {
		// Read key entered
		scanf("%c",&key);
		// Process key value for direction 
		switch(key) {
			case 119:	// w key
				printf("Up");
				// Write needs port object, data pointer, and data length
				write(uartPort,&key,1);
				break;
			case 115:	// s key
				printf("Down");
				write(uartPort,&key,1);
				break;
			case 100:	// d key
				printf("Right");
				write(uartPort,&key,1);
				break;
			case 97:	// a key
				printf("Left");
				write(uartPort,&key,1);
				break;
		}
	}
	close(uartPort);
}
