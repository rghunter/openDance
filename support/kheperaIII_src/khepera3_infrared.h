/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#ifndef KHEPERA3_INFRARED
#define KHEPERA3_INFRARED

#include "i2cal.h"

//! Infrared sensors
enum eKhepera3SensorsInfrared {
	cKhepera3SensorsInfrared_BackLeft = 0,
	cKhepera3SensorsInfrared_Left = 1,
	cKhepera3SensorsInfrared_FrontSideLeft = 2,
	cKhepera3SensorsInfrared_FrontLeft = 3,
	cKhepera3SensorsInfrared_FrontRight = 4,
	cKhepera3SensorsInfrared_FrontSideRight = 5,
	cKhepera3SensorsInfrared_Right = 6,
	cKhepera3SensorsInfrared_BackRight = 7,
	cKhepera3SensorsInfrared_Back = 8,
	cKhepera3SensorsInfrared_FloorRight = 9,
	cKhepera3SensorsInfrared_FloorLeft = 10,

	cKhepera3SensorsInfrared_Begin = 0,
	cKhepera3SensorsInfrared_End = 10,
	cKhepera3SensorsInfrared_Count = 11,
	cKhepera3SensorsInfrared_RingBegin = 0,
	cKhepera3SensorsInfrared_RingEnd = 9,
	cKhepera3SensorsInfrared_RingCount = 9,
	cKhepera3SensorsInfrared_FloorBegin = 9,
	cKhepera3SensorsInfrared_FloorEnd = 10,
	cKhepera3SensorsInfrared_FloorCount = 2
};

//! One measurement of all infrared sensors
struct sKhepera3SensorsInfrared {
	int sensor[11];		//!< Sensor values
	int timestamp;		//!< Timestamp at which the values were read
};

//! (private) Initializes this submodule.
void khepera3_infrared_init();

//! (private) General function to read infrared measures. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 31 bytes.
int khepera3_infrared_p(struct sKhepera3SensorsInfrared *result, char infrared_command);

//! Reads the last ambient infrared values and fills them into the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 31 bytes.
int khepera3_infrared_ambient();
//! Reads the last ambient infrared values and fills them into the provided structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 31 bytes.
int khepera3_infrared_ambient_p(struct sKhepera3SensorsInfrared *result);
//! Reads the last infrared proximity values and fills them into the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 31 bytes.
int khepera3_infrared_proximity();
//! Reads the last infrared proximity values and fills them into the provided structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 31 bytes.
int khepera3_infrared_proximity_p(struct sKhepera3SensorsInfrared *result);

#endif
