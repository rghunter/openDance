/*!
 * (c) 2006-2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include "khepera3.h"
#include "commandline.h"
#include <stdio.h>
#include <stdlib.h>

// Prints the help text.
void help() {
	printf("Sets the motor encoder values.\n");
	printf("\n");
	printf("Usage:\n");
	printf("  motor_setposition POS         Sets both motors to POS\n");
	printf("  motor_setposition POS1 POS2   Sets the left motor to POS1 and the right motor to POS2\n");
	printf("  motor_setposition -l POS      Sets the left motor to POS\n");
	printf("  motor_setposition -r POS      Sets the right motor to POS\n");
	printf("\n");
}

// Main program.
int main(int argc, char *argv[]) {
	// Command line parsing
	commandline_init();
	commandline_parse(argc, argv);

	// Help
	if (commandline_option_provided("-h", "--help")) {
		help();
		exit(1);
	}

	// Initialization
	khepera3_init();

	// Put the wheels in stop mode
	khepera3_drive_stop();

	// Set the position
	if (commandline_argument_count() == 1) {
		khepera3_drive_set_current_position(commandline_argument_int(0, 0), commandline_argument_int(0, 0));
	} else if (commandline_argument_count() == 2) {
		khepera3_drive_set_current_position(commandline_argument_int(0, 0), commandline_argument_int(1, 0));
	} else {
		if (commandline_option_provided("-l", "--left")) {
			khepera3_motor_set_current_position(&(khepera3.motor_left), commandline_option_value_int("-l", "--left", 0));
		}

		if (commandline_option_provided("-r", "--right")) {
			khepera3_motor_set_current_position(&(khepera3.motor_right), commandline_option_value_int("-r", "--right", 0));
		}
	}

	return 0;
}
