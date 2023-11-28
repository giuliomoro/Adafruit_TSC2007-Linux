/*
  Wire.cpp - wrapper over mbed I2C / I2CSlave
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2018-2019 Arduino SA

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "Wire.h"

arduino::MbedI2C::MbedI2C(unsigned int bus) : bus(bus), usedTxBuffer(0) {}

void arduino::MbedI2C::begin() {
	end();
	master = new I2c;
	master->initI2C_RW(bus, 1, -1);
}

#ifdef DEVICE_I2CSLAVE
void arduino::MbedI2C::begin(uint8_t slaveAddr) {
	end();
	slave = new mbed::I2CSlave((PinName)_sda, (PinName)_scl);
	slave->address(slaveAddr << 1);
	slave_th = new rtos::Thread(osPriorityNormal, 2048, nullptr, "I2CSlave");
	slave_th->start(mbed::callback(this, &arduino::MbedI2C::receiveThd));
}
#endif

void arduino::MbedI2C::end() {
	if (master != NULL) {
		delete master;
		master = NULL;
	}
#ifdef DEVICE_I2CSLAVE
	if (slave != NULL) {
		slave_th->terminate();
		slave_th->free_stack();
		delete slave_th;
		delete slave;
		slave = NULL;
	}
#endif
}

void arduino::MbedI2C::setClock(uint32_t freq) {
	fprintf(stderr, "Unable to set I2C bus frequency (%u Hz requested). The default value will be used\n", freq);
#ifdef DEVICE_I2CSLAVE
	if (slave != NULL) {
		slave->frequency(freq);
	}
#endif
}

void arduino::MbedI2C::beginTransmission(uint8_t address) {
	_address = address;
	usedTxBuffer = 0;
}

uint8_t arduino::MbedI2C::endTransmission(bool stopBit) {
	#ifndef TARGET_PORTENTA_H7
	if (usedTxBuffer == 0) {
		// we are scanning, return 0 if the addresed device responds with an ACK
		char buf[1];
		int ret = master->readRaw(_address, (i2c_char_t*)buf, 1, !stopBit);
		return ret;
	}
	#endif
	if (master->writeRaw(_address, (i2c_char_t *) txBuffer, usedTxBuffer, !stopBit) == usedTxBuffer) return 0;
	return 2;
}

uint8_t arduino::MbedI2C::endTransmission(void) {
	return endTransmission(true);
}

size_t arduino::MbedI2C::requestFrom(uint8_t address, size_t len, bool stopBit) {
	i2c_char_t buf[256];
	int ret = master->readRaw(address, buf, len, !stopBit) == len ? 0 : -1;
	if (ret != 0) {
		return 0;
	}
	for (size_t i=0; i<len; i++) {
		rxBuffer.store_char(buf[i]);
	}
	return len;
}

size_t arduino::MbedI2C::requestFrom(uint8_t address, size_t len) {
	return requestFrom(address, len, true);
}

size_t arduino::MbedI2C::write(uint8_t data) {
	if (usedTxBuffer == 256) return 0;
	txBuffer[usedTxBuffer++] = data;
	return 1;
}

size_t arduino::MbedI2C::write(const uint8_t* data, int len) {
	if (usedTxBuffer + len > 256) len = 256 - usedTxBuffer;
	memcpy(txBuffer + usedTxBuffer, data, len);
	usedTxBuffer += len;
	return len;
}

int arduino::MbedI2C::read() {
	if (rxBuffer.available()) {
		return rxBuffer.read_char();
	}
	return -1;
}

int arduino::MbedI2C::available() {
	return rxBuffer.available();
}

int arduino::MbedI2C::peek() {
	return rxBuffer.peek();
}

void arduino::MbedI2C::flush() {
}

#ifdef DEVICE_I2CSLAVE
void arduino::MbedI2C::receiveThd() {
	while (1) {
		int i = slave->receive();
		int c = 0;
		int buf_idx = 0;
		switch (i) {
			case mbed::I2CSlave::ReadAddressed:
				if (onRequestCb != NULL) {
					onRequestCb();
				}
				if (usedTxBuffer != 0) {
					slave->write((const char *) txBuffer, usedTxBuffer);
					usedTxBuffer = 0;
				}
				//slave->stop();
				break;
			case mbed::I2CSlave::WriteGeneral:
			case mbed::I2CSlave::WriteAddressed:
				rxBuffer.clear();
				char buf[240];
				c = slave->read(buf, sizeof(buf));
				for (buf_idx = 0; buf_idx < c; buf_idx++) {
					if (rxBuffer.availableForStore()) {
						rxBuffer.store_char(uint8_t(buf[buf_idx]));
					} else {
						break;
					}
				}
				if (rxBuffer.available() > 0 && onReceiveCb != NULL) {
					onReceiveCb(rxBuffer.available());
				}
				//slave->stop();
				break;
		case mbed::I2CSlave::NoData:
			//slave->stop();
			yield();
			break;
		}
	}
}
#endif

void arduino::MbedI2C::onReceive(voidFuncPtrParamInt cb) {
	onReceiveCb = cb;
}
void arduino::MbedI2C::onRequest(voidFuncPtr cb) {
	onRequestCb = cb;
}


#if WIRE_HOWMANY > 0
arduino::MbedI2C Wire0(1);
#endif
#if WIRE_HOWMANY > 1
arduino::MbedI2C Wire(1); // I2C1 default for BBB
#endif
#if WIRE_HOWMANY > 2
arduino::MbedI2C Wire2(2);
#endif
