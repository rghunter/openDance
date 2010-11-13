/*!
 * (c) 2006-2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include "khepera3.h"
#include "commandline.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Prints the help text.
void help() {
	printf("Runs the motors to a certain encoder position.\n");
	printf("\n");
	printf("Usage:\n");
	printf("  motor_gotoposition [OPTIONS] POS         Runs both motors to POS\n");
	printf("  motor_gotoposition [OPTIONS] POS1 POS2   Runs the left motor until POS1 and right motor to POS2\n");
	printf("  motor_gotoposition [OPTIONS] -l POS      Runs the left motor to POS\n");
	printf("  motor_gotoposition [OPTIONS] -r POS      Runs the right motor to POS\n");
	printf("\n");
	printf("Options:\n");
	printf("  -p     Uses a trapezoidal speed profile\n");
	printf("\n");
}

// Main program.
int main(int argc, char *argv[]) {
	// Command line parsing
	commandline_init();
	commandline_option_register("-p", "--profile", cCommandLine_Option);
	commandline_parse(argc, argv);

	// Help
	if (commandline_option_provided("-h", "--help")) {
		help();
		exit(1);
	}

	// Initialization
	khepera3_init();

	// Put the wheels in normal (control) mode
	khepera3_drive_start();

	// Set the speed
	int left, right;
	if (commandline_argument_count() == 1) {
		left = commandline_argument_int(0,0);
		right = left;
		if (commandline_option_provided("-p", "--profile")) {
			khepera3_drive_goto_position_using_profile(commandline_argument_int(0, 0), commandline_argument_int(0, 0));
		} else {
			khepera3_drive_goto_position(commandline_argument_int(0, 0), commandline_argument_int(0, 0));
		}
	} else if (commandline_argument_count() == 2) {
		left = commandline_argument_int(0,0);
		right = commandline_argument_int(1,0);
		if (commandline_option_provided("-p", "--profile")) {
			khepera3_drive_goto_position_using_profile(commandline_argument_int(0, 0), commandline_argument_int(1, 0));
		} else {
			khepera3_drive_goto_position(commandline_argument_int(0, 0), commandline_argument_int(1, 0));
		}
	} else {
		if (commandline_option_provided("-l", "--left")) {
			left = commandline_option_value_int("-l","--left",0);
			right = -1; //TODO reserved value
			if (commandline_option_provided("-p", "--profile")) {
				khepera3_motor_goto_position_using_profile(&(khepera3.motor_left), commandline_option_value_int("-l", "--left", 0));
			} else {
				khepera3_motor_goto_position(&(khepera3.motor_left), commandline_option_value_int("-l", "--left", 0));
			}
		}

		if (commandline_option_provided("-r", "--right")) {
			left = -1;
			right = commandline_option_value_int("-r","--right",0);
			if (commandline_option_provided("-p", "--profile")) {
				khepera3_motor_goto_position_using_profile(&(khepera3.motor_right), commandline_option_value_int("-r", "--right", 0));
			} else {
				khepera3_motor_goto_position(&(khepera3.motor_right), commandline_option_value_int("-r", "--right", 0));
			}
		}
	}

	for(;;)
	{

		khepera3_motor_get_current_position(&(khepera3.motor_right));
		khepera3_motor_get_current_position(&(khepera3.motor_left));

		if(left == -1)
		{
			if(right == khepera3.motor_right.current_position)
				return 0;

		}else if(right == -1)
		{
			if(left == -khepera3.motor_left.current_position)
				return 0;

		}else
		{
			if((left == -khepera3.motor_left.current_position) && (right == khepera3.motor_right.current_position))
				return 0;
		}


		usleep(1000);
	}


	return 0;
}

