/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include <assert.h>
#include "khepera3_battery.h"
#include "khepera3.h"

/*** Module initialization ***/

void khepera3_battery_init() {
}

/*** Low-level functions ***/

struct i2c_msg *khepera3_battery_p(char argument) {
	struct i2c_msg *msg_read;

	// Send command and read result
	i2cal_start();
	i2cal_writedata_uint8(3);
	i2cal_writedata_uint8('V');
	i2cal_writedata_uint8(argument);
	i2cal_write(khepera3.dspic.i2c_address);
	msg_read = i2cal_read(khepera3.dspic.i2c_address, 7);
	if (i2cal_commit() < 0) {
		return 0;
	}
	
	return msg_read;
}

int khepera3_battery_signed_p(int *result, char argument) {
	struct i2c_msg *msg_read;
	int sign = 1;

	// Read the message
	msg_read=khepera3_battery_p(argument);
	if (msg_read == 0) {
		return 0;
	}

	// We need to do some fancy decoding for the sign - people actually reinvented signed integers here :-)
	if (msg_read->buf[4] & 0x80) {
		msg_read->buf[4] &= 0x7f;
		sign = -1;
	}

	// Decode the result
	//printf("%u, %u, %u, %u\n", msg_read->buf[3], msg_read->buf[4], msg_read->buf[5], msg_read->buf[6]);
	*result = sign * (i2cal_readdata_uint16(msg_read, 3) * 10000 + i2cal_readdata_uint16(msg_read, 5));
	return -1;
}

int khepera3_battery_unsigned_p(unsigned int *result, char argument) {
	struct i2c_msg *msg_read;

	// Read the message
	msg_read=khepera3_battery_p(argument);
	if (msg_read == 0) {
		return 0;
	}

	// Decode the result
	//printf("%u, %u, %u, %u\n", msg_read->buf[3], msg_read->buf[4], msg_read->buf[5], msg_read->buf[6]);
	*result = (i2cal_readdata_uint16(msg_read, 3) * 10000 + i2cal_readdata_uint16(msg_read, 5));
	return -1;
}

/*** High-level functions ***/

int khepera3_battery_voltage() {
	return khepera3_battery_unsigned_p(&(khepera3.battery.voltage), 0);
}

int khepera3_battery_current() {
	return khepera3_battery_signed_p(&(khepera3.battery.current), 1);
}

int khepera3_battery_current_average() {
	return khepera3_battery_signed_p(&(khepera3.battery.current_average), 2);
}

int khepera3_battery_capacity_remaining_absolute() {
	return khepera3_battery_unsigned_p(&(khepera3.battery.capacity_remaining_absolute), 3);
}

int khepera3_battery_capacity_remaining_relative() {
	return khepera3_battery_unsigned_p(&(khepera3.battery.capacity_remaining_relative), 5);
}

int khepera3_battery_temperature() {
	return khepera3_battery_unsigned_p(&(khepera3.battery.temperature), 4);
}
