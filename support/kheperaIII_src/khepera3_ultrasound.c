/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "khepera3_ultrasound.h"
#include "khepera3.h"

/*** Constants ***/

const char *cKhepera3SensorsUltrasound[5] = {"left", "frontleft", "front", "frontright", "right"};

/*** Module initialization ***/

void khepera3_ultrasound_init() {
	khepera3.ultrasound.distance_per_increment = 0.01;
}

/*** Low-level functions ***/

int khepera3_ultrasound_p(struct sKhepera3SensorsUltrasoundSensor *result, enum eKhepera3SensorsUltrasound sensor) {
	struct i2c_msg *msg_read;
	int i;

	// Send command and read result
	i2cal_start();
	i2cal_writedata_uint8(3);
	i2cal_writedata_uint8('G');
	i2cal_writedata_uint8(sensor + 1);
	i2cal_write(khepera3.dspic.i2c_address);
	msg_read = i2cal_read(khepera3.dspic.i2c_address, 47);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Check if we got a valid result
	if (i2cal_readdata_uint8(msg_read, 0) != 45) {
		return 0;
	}
	if (i2cal_readdata_uint8(msg_read, 2) != 103) {
		return 0;
	}

	// Decode number of echos
	result->echos_count = i2cal_readdata_uint8(msg_read, 3);
	if (result->echos_count > 10) {
		result->echos_count = 0;
	}

	// Decode distances and amplitudes
	for (i = 0; i < result->echos_count; i++) {
		result->distance[i] = i2cal_readdata_uint16(msg_read, i * 8 + 5);
		result->amplitude[i] = i2cal_readdata_uint16(msg_read, i * 8 + 7);
		result->timestamp[i] = i2cal_readdata_uint32(msg_read, i * 8 + 9);
	}

	return -1;
}

/*** High-level functions ***/

int khepera3_ultrasound(enum eKhepera3SensorsUltrasound sensor) {
	assert(sensor >= 0);
	assert(sensor <= 5);
	return khepera3_ultrasound_p(&(khepera3.ultrasound.sensor[sensor]), sensor);
}

int khepera3_ultrasound_enable(enum eKhepera3SensorsUltrasoundBit bitmask) {
	struct i2c_msg *msg_read;

	i2cal_start();
	i2cal_writedata_uint8(5);
	i2cal_writedata_uint8('C');
	i2cal_writedata_uint8(0);
	i2cal_writedata_uint8(0);
	i2cal_writedata_uint8(bitmask);
	i2cal_write(khepera3.dspic.i2c_address);
	msg_read = i2cal_read(khepera3.dspic.i2c_address, 1);	// The dsPIC wants us to read one byte in order to confirm the command
	if (i2cal_commit() < 0) {
		return 0;
	}
	return -1;
}

int khepera3_ultrasound_set_max_echo_number(int max_echo_number) {
	struct i2c_msg *msg_read;

	i2cal_start();
	i2cal_writedata_uint8(5);
	i2cal_writedata_uint8('C');
	i2cal_writedata_uint8(254);
	i2cal_writedata_uint8(0);
	i2cal_writedata_uint8(max_echo_number);
	i2cal_write(khepera3.dspic.i2c_address);
	msg_read = i2cal_read(khepera3.dspic.i2c_address, 1);	// The dsPIC wants us to read one byte in order to confirm the command
	if (i2cal_commit() < 0) {
		return 0;
	}
	return -1;
}

enum eKhepera3SensorsUltrasound khepera3_ultrasound_getsensorbyname(const char *name, enum eKhepera3SensorsUltrasound defaultvalue) {
	int i;
	if (name == 0) {
		return defaultvalue;
	}

	// By number (only if it starts with a digit)
	if ((name[0] >= '0') && (name[0] <= '9')) {
		i = strtol(name, 0, 0);
		if ((i < 0) || (i > 4)) {
			return defaultvalue;
		}
		return i;
	}

	// By name
	for (i = 0; i < 5; i++) {
		if (strcmp(name, cKhepera3SensorsUltrasound[i]) == 0) {
			return i;
		}
	}

	// Nothing
	return defaultvalue;
}

enum eKhepera3SensorsUltrasoundBit khepera3_ultrasound_getsensorbitbysensor(enum eKhepera3SensorsUltrasound sensor) {
	return (1 << sensor);
}

