#include "Adafruit_I2CDevice.h"

Adafruit_I2CDevice::Adafruit_I2CDevice(uint8_t address, TwoWire* wire)
{
	initI2C_RW(wire == &Wire ? 1 : 2, address, -1);
}

Adafruit_I2CDevice::~Adafruit_I2CDevice()
{
}

int Adafruit_I2CDevice::begin()
{
	return 0;
}
int Adafruit_I2CDevice::write(uint8_t* bytes, size_t len)
{
	return len == writeBytes(bytes, len);
}

int Adafruit_I2CDevice::read(uint8_t* bytes, size_t len)
{
	return len == readBytes(bytes, len);
}
