#include "Adafruit_TSC2007.h"
#include <libraries/OscSender/OscSender.h>

// hostAddress should be "localhost" if sending to another application on Bela
// or 192.168.7.1 or 192.168.6.1 if sending to a port on the host computer
const char* hostAddress = "localhost";
int hostPort = 1234;
unsigned int sleepMs = 50; // sleep time between reads

int main()
{
	Adafruit_TSC2007 touch;
	OscSender oscSender(hostPort, hostAddress);
	if (!touch.begin()) {
		fprintf(stderr, "Couldn't find touch controller\n");
		return 1;
	}
	printf("Found touch controller\n");
	printf("Sending to %s:%d\n", hostAddress, hostPort);
	while(1)
	{
		uint16_t x, y, z1, z2;
		if(touch.read_touch(&x, &y, &z1, &z2)) {
			oscSender.newMessage("/tsc");
			oscSender.add(x).add(y).add(z1).add(z2).send();
		}
		usleep(sleepMs * 1000);
	}
	return 0;
}
