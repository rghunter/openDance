/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include "khepera3_motor.h"
#include "i2cal.h"

/*** Module initialization ***/

void khepera3_motor_init() {
}

/*** Register read ***/

int khepera3_motor_read_register8_p(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister8 reg, unsigned int *result) {
	struct i2c_msg *msg_read;

	// Send register and read value
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Parse value and return
	//printf("motor read %d\n", msg_read->buf[0]);
	*result = msg_read->buf[0];
	return -1;
}

int khepera3_motor_read_register16_p(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister16 reg, unsigned int *result) {
	struct i2c_msg *msg_read;
	unsigned int value;

	// Note that this should ideally be a single commit - one of the following possibilites:
	//   (1) S write reg S read 2 P
	//   (2) S write reg1 S read 1 [P] S write reg2 S read 1 P
	// Method (1) doesn't work because the firmware only allows reading/writing 8 bit messages at a time.
	// Method (2) doesn't work for some reason because the Linux Kernel 2.4 currently in use on the robots seems to generate wrong signals then. In addition, there is no way to generate the stop bit [P] between the two register reads while keeping it in one commit. The only solution would be to synchronize using something else (e.g. file lock).

	// Send register and read value (for H register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 1);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}
	value = msg_read->buf[0] << 8;
	//printf("motor read H %d\n", msg_read->buf[0]);

	// Send register and read value (for L register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}
	value |= msg_read->buf[0];
	//printf("motor read L %d\n", msg_read->buf[0]);

	// Return success1
	*result = value;
	return -1;
}

int khepera3_motor_read_register32_p(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister32 reg, unsigned int *result) {
	struct i2c_msg *msg_read;
	unsigned int value;

	// See notes in khepera3_motor_read_register16_p

	// Send register and read value (for HH register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 3);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}
	value = msg_read->buf[0] << 24;
	//printf("motor read HH %d\n", msg_read->buf[0]);

	// Send register and read value (for HL register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 2);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}
	value |= msg_read->buf[0] << 16;
	//printf("motor read HL %d\n", msg_read->buf[0]);

	// Send register and read value (for LH register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 1);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}
	value |= msg_read->buf[0] << 8;
	//printf("motor read LH %d\n", msg_read->buf[0]);

	// Send register and read value (for LL register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg);
	i2cal_write(motor->i2c_address);
	msg_read = i2cal_read(motor->i2c_address, 1);
	if (i2cal_commit() < 0) {
		return 0;
	}
	value |= msg_read->buf[0];
	//printf("motor read LL %d\n", msg_read->buf[0]);

	// Return success
	*result = value;
	return -1;
}

/*** Register write ***/

int khepera3_motor_write_register8(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister8 reg, unsigned int value) {
	// Send register and read value
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg);
	i2cal_writedata_uint8(value);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	return -1;
}

int khepera3_motor_write_register16(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister16 reg, unsigned int value) {
	// Send register and read value (for H register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 1);
	i2cal_writedata_uint8((value >> 8) & 0xff);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Send register and read value (for L register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg);
	i2cal_writedata_uint8(value & 0xff);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	return -1;
}

int khepera3_motor_write_register32(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister32 reg, unsigned int value) {
	// Send register and read value (for HH register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 3);
	i2cal_writedata_uint8((value >> 24) & 0xff);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Send register and read value (for HL register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 2);
	i2cal_writedata_uint8((value >> 16) & 0xff);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Send register and read value (for LH register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg + 1);
	i2cal_writedata_uint8((value >> 8) & 0xff);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	// Send register and read value (for LL register)
	i2cal_start();
	i2cal_writedata_uint8((unsigned int)reg);
	i2cal_writedata_uint8(value & 0xff);
	i2cal_write(motor->i2c_address);
	if (i2cal_commit() < 0) {
		return 0;
	}

	return -1;
}

/*** Utility ***/

int khepera3_motor_initialize(struct sKhepera3Motor *motor) {
	int res = -1;

	// Read firmware version
	khepera3_motor_firmware_version(motor);

	// General configuration
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_Mode, cKhepera3MotorMode_Idle);
	res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_SampleTime, 1550);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_NearTargetMargin, 6);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_HardwareOptions, 0);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_SoftwareOptions, cKhepera3MotorSoftwareOptions_AntiResetWindup | cKhepera3MotorSoftwareOptions_StopIfBlocked);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_ErrorFlags, 0);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_BlockedTime, 10);
	res &= khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_SoftStopMin, -10000);
	res &= khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_SoftStopMax, 10000);
	res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_SoftwareCurrentLimit, 500);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_SetpointSource, cKhepera3MotorSetpointSource_I2C);

	// PID configuration
	if (motor->firmware_version >= 0x68) {
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KpPosition, 600);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KdPosition, 20);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KiPosition, 30);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KpSpeed, 620);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KdSpeed, 3);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KiSpeed, 10);
	} else {
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KpPosition, 620);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KdPosition, 3);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KiPosition, 10);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KpSpeed, 170);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KdSpeed, 0);
		res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_KiSpeed, 40);
	}

	// Trapezeoidal speed profile
	res &= khepera3_motor_write_register16(motor, cKhepera3MotorRegister16_MaxSpeed, 10000);
	res &= khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_Acceleration, 128);

	return res;
}

int khepera3_motor_set_control_type(struct sKhepera3Motor *motor, enum eKhepera3MotorControlType control_type) {
	if (motor->control_type == control_type) {
		return -1;
	}

	motor->control_type = control_type;
	if (motor->control_type == cKhepera3MotorControlType_Unknown) {
		return -1;
	}

	return khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_ControlType, motor->control_type);
}

/*** Set values ***/

int khepera3_motor_stop(struct sKhepera3Motor *motor) {
	return khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_Mode, cKhepera3MotorMode_StopMotor);
}

int khepera3_motor_start(struct sKhepera3Motor *motor) {
	return khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_Mode, cKhepera3MotorMode_Normal);
}

int khepera3_motor_idle(struct sKhepera3Motor *motor) {
	return khepera3_motor_write_register8(motor, cKhepera3MotorRegister8_Mode, cKhepera3MotorMode_Idle);
}

int khepera3_motor_set_speed(struct sKhepera3Motor *motor, int speed) {
	int res;
	res = khepera3_motor_set_control_type(motor, cKhepera3MotorControlType_Speed);
	if (res == 0) {
		return 0;
	}
	return khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_Setpoint, speed * motor->direction);
}

int khepera3_motor_set_speed_using_profile(struct sKhepera3Motor *motor, int speed) {
	int res;
	res = khepera3_motor_set_control_type(motor, cKhepera3MotorControlType_SpeedUsingProfile);
	if (res == 0) {
		return 0;
	}
	return khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_Setpoint, speed * motor->direction);
}

int khepera3_motor_goto_position(struct sKhepera3Motor *motor, int position) {
	int res;
	res = khepera3_motor_set_control_type(motor, cKhepera3MotorControlType_Position);
	if (res == 0) {
		return 0;
	}
	return khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_Setpoint, position * motor->direction);
}

int khepera3_motor_goto_position_using_profile(struct sKhepera3Motor *motor, int position) {
	int res;
	res = khepera3_motor_set_control_type(motor, cKhepera3MotorControlType_PositionUsingProfile);
	if (res == 0) {
		return 0;
	}
	return khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_Setpoint, position * motor->direction);
}

int khepera3_motor_set_current_position(struct sKhepera3Motor *motor, int position) {
	motor->current_position = position;
	return khepera3_motor_write_register32(motor, cKhepera3MotorRegister32_CurrentPosition, motor->current_position);
}

/*** Get values ***/

int khepera3_motor_firmware_version(struct sKhepera3Motor *motor) {
	int res;
	unsigned int value;

	res = khepera3_motor_read_register8_p(motor, cKhepera3MotorRegister8_FirmwareVersion, &value);
	if (res == 0) {
		return 0;
	}

	motor->firmware_version = value;
	return -1;
}

int khepera3_motor_get_status(struct sKhepera3Motor *motor) {
	int res;
	unsigned int value;

	res = khepera3_motor_read_register8_p(motor, cKhepera3MotorRegister32_Setpoint, &value);
	if (res == 0) {
		return 0;
	}

	motor->status = value;
	return -1;
}

int khepera3_motor_get_error(struct sKhepera3Motor *motor) {
	int res;
	unsigned int value;

	res = khepera3_motor_read_register8_p(motor, cKhepera3MotorRegister32_Setpoint, &value);
	if (res == 0) {
		return 0;
	}

	motor->error = value;
	return -1;
}

int khepera3_motor_get_current_speed(struct sKhepera3Motor *motor) {
	return khepera3_motor_read_register32_p(motor, cKhepera3MotorRegister32_CurrentSpeed, (unsigned int *)&(motor->current_speed));
}

int khepera3_motor_get_current_position(struct sKhepera3Motor *motor) {
	return khepera3_motor_read_register32_p(motor, cKhepera3MotorRegister32_CurrentPosition, (unsigned int *)&(motor->current_position));
}

int khepera3_motor_get_current_torque(struct sKhepera3Motor *motor) {
	return khepera3_motor_read_register16_p(motor, cKhepera3MotorRegister16_CurrentTorque, (unsigned int *)&(motor->current_torque));
}
