#include "Adafruit_I2CDevice.h"

Adafruit_I2CDevice::Adafruit_I2CDevice(uint8_t address, TwoWire* wire)
{
	bus = wire == &Wire ? 1 : 2;
	addr = address;
}

Adafruit_I2CDevice::~Adafruit_I2CDevice()
{
}

bool Adafruit_I2CDevice::begin()
{
	return 0 == initI2C_RW(bus, addr, -1);
}
bool Adafruit_I2CDevice::write(uint8_t* bytes, size_t len)
{
	return len == writeBytes(bytes, len);
}

bool Adafruit_I2CDevice::read(uint8_t* bytes, size_t len)
{
	return len == readBytes(bytes, len);
}
