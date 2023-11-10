#pragma once
#include <Wire.h>
#include <I2c.h>
#include <stdint.h>

class Adafruit_I2CDevice : I2c
{
public:
	Adafruit_I2CDevice(uint8_t, TwoWire*);
	~Adafruit_I2CDevice();
	bool begin();
	bool write(uint8_t*, size_t);
	bool read(uint8_t*, size_t);
	bool write_then_read(const uint8_t *write_buffer, size_t write_len,
		uint8_t *read_buffer, size_t read_len,
		bool stop = false);
private:
	int bus;
	int addr;

};
