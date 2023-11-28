/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include "Arduino.h"
#include "Stream.h"
#include "I2c.h"
// NOTE: MbedI2C is actually LinuxI2C
#define WIRE_HOWMANY 3
#ifdef DEVICE_I2CSLAVE
#include "drivers/I2CSlave.h"
#endif

typedef void (*voidFuncPtrParamInt)(int);
typedef void (*voidFuncPtr)();

#include "RingBuffer.h"
namespace arduino {

#if 0
template<size_t n>
class RingBufferN {
		rxBuffer.store_char(buf[i]);
	if (rxBuffer.available()) {
	return rxBuffer.peek();
				rxBuffer.clear();
					if (rxBuffer.availableForStore()) {
						rxBuffer.store_char(uint8_t(buf[buf_idx]));
		return rxBuffer.read_char();
};
#endif
class MbedI2C : public Stream
{
  public:
    MbedI2C(unsigned int bus);
    virtual void begin();
    #ifdef DEVICE_I2CSLAVE
    virtual void begin(uint8_t address);
    #endif
    virtual void end();

    virtual void setClock(uint32_t freq);
  
    virtual void beginTransmission(uint8_t address);
    virtual uint8_t endTransmission(bool stopBit);
    virtual uint8_t endTransmission(void);

    virtual size_t requestFrom(uint8_t address, size_t len, bool stopBit);
    virtual size_t requestFrom(uint8_t address, size_t len);

    virtual void onReceive(void(*)(int));
    virtual void onRequest(void(*)(void));

    virtual size_t write(uint8_t data);
    virtual size_t write(int data) {
      return write ((uint8_t)data);
    };
    virtual size_t write(const uint8_t* data, int len);
    using Print::write;
    virtual int read();
    virtual int peek();
    virtual void flush();
    virtual int available();

private:

#ifdef DEVICE_I2CSLAVE
    I2CSlave* slave = NULL;
#endif
    I2c* master = NULL;
    unsigned int bus;
    int _address;
    RingBufferN<256> rxBuffer;
    uint8_t txBuffer[256];
    uint32_t usedTxBuffer;
    voidFuncPtrParamInt onReceiveCb = NULL;
    voidFuncPtr onRequestCb = NULL;
#ifdef DEVICE_I2CSLAVE
    rtos::Thread* slave_th;
    void receiveThd();
#endif
};

}

#if WIRE_HOWMANY > 0
extern arduino::MbedI2C Wire;
#endif
#if WIRE_HOWMANY > 1
extern arduino::MbedI2C Wire1;
#endif
#if WIRE_HOWMANY > 2
extern arduino::MbedI2C Wire2;
#endif

typedef arduino::MbedI2C TwoWire;
