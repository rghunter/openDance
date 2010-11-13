/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#ifndef KHEPERA3
#define KHEPERA3

#include "i2cal.h"
#include "khepera3_motor.h"
#include "khepera3_drive.h"
#include "khepera3_infrared.h"
#include "khepera3_ultrasound.h"
#include "khepera3_battery.h"

//! dsPIC microcontroller
struct sKhepera3DSPIC {
	int i2c_address;					//!< I2C bus address
	unsigned int firmware_version;		//!< Firmware version
	unsigned int firmware_revision;		//!< Firmware revision
};

//! The complete Khepera 3 robot
struct sKhepera3 {
	struct sKhepera3DSPIC dspic;							//!< dsPIC microcontroller
	struct sKhepera3Motor motor_left;						//!< Left motor
	struct sKhepera3Motor motor_right;						//!< Right motor
	struct sKhepera3SensorsInfrared infrared_ambient;		//!< Infrared sensors in ambient light mode
	struct sKhepera3SensorsInfrared infrared_proximity;		//!< Infrared sensors in proximity mode
	struct sKhepera3SensorsUltrasound ultrasound;			//!< Ultrasound sensors
	struct sKhepera3Battery battery;						//!< Battery information
};

struct sKhepera3 khepera3;

//! Initializes this module.
void khepera3_init();

//! Resets the timestamp on the dsPIC. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 3 bytes.
int khepera3_timestamp_reset();
//! Reads the firmware version and revision and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 11 bytes.
int khepera3_dspic_firmware_version();

#endif
