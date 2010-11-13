/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#ifndef KHEPERA3_DRIVE
#define KHEPERA3_DRIVE

#include "i2cal.h"

//! (private) Initializes this submodule.
void khepera3_drive_init();

//! Stops both motors immediately. Transfer on I2C bus: 6 bytes.
void khepera3_drive_stop();
//! Puts both motors in normal (control) mode. Transfer on I2C bus: 6 bytes.
void khepera3_drive_start();
//! Puts both motors in idle mode. Transfer on I2C bus: 6 bytes.
void khepera3_drive_idle();

//! Sets the motor speeds. Transfer on I2C bus: 24 - 30 bytes.
void khepera3_drive_set_speed(int speed_left, int speed_right);
//! Sets the motor speeds using forward and a differential coefficient. The speed of the left wheel is set to speed * (forward_coefficient + differential_coefficient), whereas the right wheel speed is set to speed * (forward_coefficient - differential_coefficient). Transfer on I2C bus: 24 - 30 bytes.
void khepera3_drive_set_speed_differential(float speed, float forward_coefficient, float differential_coefficient);
//! Same as khepera3_drive_set_speed_differential, but the coefficients are bounded by -*_coefficient_max and *_coefficient_max. Transfer on I2C bus: 24 - 30 bytes.
void khepera3_drive_set_speed_differential_bounded(float speed, float forward_coefficient, float forward_coefficient_max, float differential_coefficient, float differential_coefficient_max);
//! Sets the motor speeds and uses the acceleration profile to reach that speed. Transfer on I2C bus: 24 - 30 bytes.
void khepera3_drive_set_speed_using_profile(int speed_left, int speed_right);

//! Goes to a specific motor position. Transfer on I2C bus: 24 - 30 bytes.
void khepera3_drive_goto_position(int position_left, int position_right);
//! Goes to a specific motor position using the acceleration profile. Transfer on I2C bus: 24 - 30 bytes.
void khepera3_drive_goto_position_using_profile(int position_left, int position_right);

//! Sets the current encoder position. Transfer on I2C bus: 24 bytes.
void khepera3_drive_set_current_position(int position_left, int position_right);

//! Reads the current speed and updates the corresponding value the motor structures. Transfer on I2C bus: 32 bytes.
void khepera3_drive_get_current_speed();
//! Reads the current position and updates the corresponding value the motor structures. Transfer on I2C bus: 32 bytes.
void khepera3_drive_get_current_position();
//! Reads the current torque and updates the corresponding value the motor structures. Transfer on I2C bus: 32 bytes.
void khepera3_drive_get_current_torque();

#endif
