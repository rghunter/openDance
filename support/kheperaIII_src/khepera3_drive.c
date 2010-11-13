/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include <assert.h>
#include <math.h>
#include "khepera3_drive.h"
#include "khepera3.h"

/*** Module initialization ***/

void khepera3_drive_init() {
	khepera3_motor_init(&(khepera3.motor_left));
	khepera3_motor_init(&(khepera3.motor_right));
	khepera3.motor_left.direction = -1;
	khepera3.motor_right.direction = 1;
}

/*** Functions to set/get values on both motors ***/

void khepera3_drive_stop() {
	khepera3_motor_stop(&(khepera3.motor_left));
	khepera3_motor_stop(&(khepera3.motor_right));
}

void khepera3_drive_start() {
	khepera3_motor_start(&(khepera3.motor_left));
	khepera3_motor_start(&(khepera3.motor_right));
}

void khepera3_drive_idle() {
	khepera3_motor_idle(&(khepera3.motor_left));
	khepera3_motor_idle(&(khepera3.motor_right));
}

void khepera3_drive_set_speed(int speed_left, int speed_right) {
	khepera3_motor_set_speed(&(khepera3.motor_left), speed_left);
	khepera3_motor_set_speed(&(khepera3.motor_right), speed_right);
}

void khepera3_drive_set_speed_differential(float speed, float forward_coefficient, float differential_coefficient) {
	float speed_left;
	float speed_right;

	speed_left = speed * (forward_coefficient + differential_coefficient);
	speed_right = speed * (forward_coefficient - differential_coefficient);
	//printf("$SPEED,%f,%f\n", speed_left, speed_right);
	khepera3_drive_set_speed((int)floorf(speed_left + 0.5), (int)floorf(speed_right + 0.5));
}

void khepera3_drive_set_speed_differential_bounded(float speed, float forward_coefficient, float forward_coefficient_max, float differential_coefficient, float differential_coefficient_max) {
	if (forward_coefficient > forward_coefficient_max) {
		forward_coefficient = forward_coefficient_max;
	}
	if (forward_coefficient < -forward_coefficient_max) {
		forward_coefficient = -forward_coefficient_max;
	}

	if (differential_coefficient > differential_coefficient_max) {
		differential_coefficient = differential_coefficient_max;
	}
	if (differential_coefficient < -differential_coefficient_max) {
		differential_coefficient = -differential_coefficient_max;
	}

	khepera3_drive_set_speed_differential(speed, forward_coefficient, differential_coefficient);
}

void khepera3_drive_set_speed_using_profile(int speed_left, int speed_right) {
	khepera3_motor_set_speed_using_profile(&(khepera3.motor_left), speed_left);
	khepera3_motor_set_speed_using_profile(&(khepera3.motor_right), speed_right);
}

void khepera3_drive_goto_position(int position_left, int position_right) {
	khepera3_motor_goto_position(&(khepera3.motor_left), position_left);
	khepera3_motor_goto_position(&(khepera3.motor_right), position_right);
}

void khepera3_drive_goto_position_using_profile(int position_left, int position_right) {
	khepera3_motor_goto_position_using_profile(&(khepera3.motor_left), position_left);
	khepera3_motor_goto_position_using_profile(&(khepera3.motor_right), position_right);
}

void khepera3_drive_set_current_position(int position_left, int position_right) {
	khepera3_motor_set_current_position(&(khepera3.motor_left), position_left);
	khepera3_motor_set_current_position(&(khepera3.motor_right), position_right);
}

void khepera3_drive_get_current_speed() {
	khepera3_motor_get_current_speed(&(khepera3.motor_left));
	khepera3_motor_get_current_speed(&(khepera3.motor_right));
}

void khepera3_drive_get_current_position() {
	khepera3_motor_get_current_position(&(khepera3.motor_left));
	khepera3_motor_get_current_position(&(khepera3.motor_right));
}

void khepera3_drive_get_current_torque() {
	khepera3_motor_get_current_torque(&(khepera3.motor_left));
	khepera3_motor_get_current_torque(&(khepera3.motor_right));
}
