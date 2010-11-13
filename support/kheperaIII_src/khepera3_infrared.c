/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include <assert.h>
#include "khepera3_infrared.h"
#include "khepera3.h"

/*** Module initialization ***/

void khepera3_infrared_init() {
}

/*** Low-level functions ***/

int khepera3_infrared_p(struct sKhepera3SensorsInfrared *result, char infrared_command) {
	struct i2c_msg *msg_read;
	int i;

	// Arguments/environment checks
	assert(result != 0);
	assert((infrared_command == 'N') || (infrared_command == 'O'));

	// Send infrared command (N or O) and read 27 bytes
	i2cal_start();
	i2cal_writedata_uint8(2);
	i2cal_writedata_uint8(infrared_command);
	i2cal_write(khepera3.dspic.i2c_address);
	msg_read = i2cal_read(khepera3.dspic.i2c_address, 27);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Decode sensor values
	for (i = 0; i < 11; i++) {
		result->sensor[i] = i2cal_readdata_int16(msg_read, i * 2 + 3);
	}

	// Decode timestamp
	result->timestamp = i2cal_readdata_uint16(msg_read, 25);

	// Everything OK
	return -1;
}

/*** High-level functions ***/

int khepera3_infrared_ambient() {
	return khepera3_infrared_p(&(khepera3.infrared_ambient), 'O');
}

int khepera3_infrared_ambient_p(struct sKhepera3SensorsInfrared *result) {
	return khepera3_infrared_p(result, 'O');
}

int khepera3_infrared_proximity() {
	return khepera3_infrared_p(&(khepera3.infrared_proximity), 'N');
}

int khepera3_infrared_proximity_p(struct sKhepera3SensorsInfrared *result) {
	return khepera3_infrared_p(result, 'N');
}
