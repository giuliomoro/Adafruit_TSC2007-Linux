#pragma once

#include <stdint.h>
#define pgm_read_byte(ADDR) (*(ADDR))
#define F(...)
#ifndef NULL
#define NULL nullptr
#endif // NULL
#define PROGMEM
#include <string.h>
#include <stdlib.h>
#include <iostream>

void setup();
void loop();
struct Print {
	void begin(uint32_t rate) {};
	template <typename T>
	void print(T arg)
	{
		std::cout << arg;
	}
	template <typename T>
	void println(T arg)
	{
		std::cout << arg << "\n";
	}
	void println()
	{
		std::cout << "\n";
	}
	operator bool()
	{
		return true;
	}
	int write(uint8_t);
};

extern Print Serial;

unsigned long micros();
uint32_t millis();
uint32_t random(uint32_t max);
uint32_t random(uint32_t min, uint32_t max);
void randomSeed(uint32_t s);
void delay(uint32_t);
void delayMicroseconds(uint32_t);
#define LED_BUILTIN 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 0
#define OUTPUT_PULLUP 0
#define LOW 0
#define HIGH 1
void pinMode(uint32_t, uint32_t);
bool digitalRead(uint32_t);
void digitalWrite(uint32_t, bool);
void pwmWrite(uint32_t, float, float = 170);
float analogRead(uint32_t);
void analogWrite(uint32_t, float);
#define LSBFIRST 0
#define MSBFIRST 1
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t bitOrder, uint8_t numBits, uint32_t val);

typedef char __FlashStringHelper;
#define strlen_P strlen
#include <Utilities.h>
void utoa(uint32_t, char*, size_t);
void noInterrupts();
void interrupts();
typedef bool boolean;

#define HEX "#x"
#define PSTR(a) (a)
#include <pthread.h>
#define yield() pthread_yield()
