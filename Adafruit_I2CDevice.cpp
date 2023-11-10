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
bool Adafruit_I2CDevice::write_then_read(const uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len, bool stop)
{
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[2];

	messages[0].addr = addr;
	messages[0].flags = 0;
	messages[0].len = write_len;
	messages[0].buf = (i2c_char_t*)write_buffer;

	// The data will get returned in this structure
	messages[1].addr = addr;
	messages[1].flags = I2C_M_RD | (stop ? 0 : I2C_M_NOSTART);
	messages[1].len = read_len;
	messages[1].buf = (i2c_char_t*)read_buffer;

	// Send the request to the kernel and get the result back
	packets.msgs = messages;
	packets.nmsgs = 2;
	if(ioctl(i2C_file, I2C_RDWR, &packets) < 0) {
		fprintf(stderr, "Failed to write_then_read\n");
		return false;
	}
	return true;
}
