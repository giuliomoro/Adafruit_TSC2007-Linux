#include "Arduino.h"

Print Serial;

uint32_t random(uint32_t max)
{
	return random(0, max);
}

void randomSeed(uint32_t s)
{
	srand(s);
}

uint32_t random(uint32_t min, uint32_t max) {
	uint32_t ran = rand();
	return map(ran, 0, (float)RAND_MAX, min, max);
}

uint32_t millis() {
	return micros() / 1000.f;
}

unsigned long micros() {
	static timespec startp;
	static bool inited;
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
	if(!inited) {
		inited = true;
		startp = tp;
		return 0;
	}
	return ((tp.tv_sec - startp.tv_sec) * 1000000 + (tp.tv_nsec - startp.tv_nsec) / 1000.0);
}

#include <unistd.h>
void delay(uint32_t t) {
	usleep(t * 1000);
}
void delayMicroseconds(uint32_t t)
{
	usleep(t);
}
void utoa(uint32_t num, char* dest, size_t len) {
	snprintf(dest, len, "%u", num);
}
void noInterrupts() {}
void interrupts() {}
