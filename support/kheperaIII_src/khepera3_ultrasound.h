/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#ifndef KHEPERA3_ULTRASOUND
#define KHEPERA3_ULTRASOUND

#include "i2cal.h"

//! Ultrasound sensors
enum eKhepera3SensorsUltrasound {
	cKhepera3SensorsUltrasound_Left = 0,
	cKhepera3SensorsUltrasound_FrontLeft = 1,
	cKhepera3SensorsUltrasound_Front = 2,
	cKhepera3SensorsUltrasound_FrontRight = 3,
	cKhepera3SensorsUltrasound_Right = 4,

	cKhepera3SensorsUltrasound_Begin = 0,
	cKhepera3SensorsUltrasound_End = 5,
	cKhepera3SensorsUltrasound_Count = 5
};

//! Ultrasound sensor bit masks
enum eKhepera3SensorsUltrasoundBit {
	cKhepera3SensorsUltrasoundBit_Left = 1,
	cKhepera3SensorsUltrasoundBit_FrontLeft = 2,
	cKhepera3SensorsUltrasoundBit_Front = 4,
	cKhepera3SensorsUltrasoundBit_FrontRight = 8,
	cKhepera3SensorsUltrasoundBit_Right = 16,

	cKhepera3SensorsUltrasoundBit_None = 0,
	cKhepera3SensorsUltrasoundBit_All = 31
};

//! Ultrasound sensor name strings
const char *cKhepera3SensorsUltrasound[5];

//! One measurement of one ultrasound sensor
struct sKhepera3SensorsUltrasoundSensor {
	int echos_count;		//!< Number of received echos
	int distance[10];		//!< Received distance in increments (cm at the moment, multiply with distance_per_increment to get meters)
	int amplitude[10];		//!< Received amplitude
	int timestamp[10];		//!< Timestamp at which the echo was received
};

//! The ultrasound sensors
struct sKhepera3SensorsUltrasound {
	struct sKhepera3SensorsUltrasoundSensor sensor[5];	//!< The 5 ultrasound sensors
	float distance_per_increment;						//!< Conversion factor to convert from distance values to meters
};

//! (private) Initializes this submodule.
void khepera3_ultrasound_init();

//! Reads the last values of an ultrasound sensor and fills them into the provided structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 52 bytes.
int khepera3_ultrasound_p(struct sKhepera3SensorsUltrasoundSensor *result, enum eKhepera3SensorsUltrasound sensor);

//! Reads the last values of an ultrasound sensor and fills them into the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 52 bytes.
int khepera3_ultrasound(enum eKhepera3SensorsUltrasound sensor);
//! Enables or disables ultrasound sensors. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 8 bytes.
int khepera3_ultrasound_enable(enum eKhepera3SensorsUltrasoundBit bitmask);
//! Sets the maximum number of echos received by the ultrasound sensors. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 8 bytes.
int khepera3_ultrasound_set_max_echo_number(int max_echo_number);
//! Returns a sensor ID by name.
enum eKhepera3SensorsUltrasound khepera3_ultrasound_getsensorbyname(const char *name, enum eKhepera3SensorsUltrasound defaultvalue);
//! Returns a sensor bitmask by sensor ID.
enum eKhepera3SensorsUltrasoundBit khepera3_ultrasound_getsensorbitbysensor(enum eKhepera3SensorsUltrasound sensor);

#endif
