/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#ifndef KHEPERA3_BATTERY
#define KHEPERA3_BATTERY

#include "i2cal.h"

//! Battery
struct sKhepera3Battery {
	unsigned int voltage;						//!< Voltage (0.1 mV)
	int current;								//!< Instantaneous current (0.1 mA)
	int current_average;						//!< Average current (0.1 mA)
	unsigned int capacity_remaining_absolute;	//!< Absolute remaining capacity (0.1 mAh)
	unsigned int capacity_remaining_relative;	//!< Relative remaining capacity (0.0001 %)
	unsigned int temperature;					//!< Temperature (0.0001 degrees Celsius)
};

//! (private) Initializes this submodule.
void khepera3_battery_init();

//! (private) Reads a value from the battery and returns a plain i2c message. Transfer on I2C bus: 12 bytes.
struct i2c_msg *khepera3_battery_p(char argument);
//! (private) Reads a signed value from the battery. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_signed_p(int *result, char argument);
//! (private) Reads an unsigned value from the battery. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_unsigned_p(unsigned int *result, char argument);

//! Reads the battery voltage and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_voltage();
//! Reads the battery current and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_current();
//! Reads the average current of the battery and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_current_average();
//! Reads the absolute remaining capacity of the battery and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_capacity_remaining_absolute();
//! Reads the relative remaining capacity of the battery and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_capacity_remaining_relative();
//! Reads the battery temperature and updates this value in the khepera3 structure. The return value indicates success (-1) or failure (0). Transfer on I2C bus: 12 bytes.
int khepera3_battery_temperature();

#endif
