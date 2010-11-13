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
	printf("Initializes the motors (PID, mode, acceleration profile, ...).\n");
	printf("This program must be run before the motors are used.\n");
	printf("\n");
	printf("Usage: motor_initialize [OPTIONS]\n");
	printf("\n");
	printf("Options:\n");
	printf("  -l --only-left    Initialize the left motor only\n");
	printf("  -r --only-right   Initialize the right motor only\n");
	printf("\n");
}

// Main program.
int main(int argc, char *argv[]) {
	// Command line parsing
	commandline_init();
	commandline_option_register("-l", "--only-left", cCommandLine_Option);
	commandline_option_register("-r", "--only-right", cCommandLine_Option);
	commandline_parse(argc, argv);

	// Help
	if (commandline_option_provided("-h", "--help")) {
		help();
		exit(1);
	}

	// Initialization
	khepera3_init();

	// Initialize left motor
	if (! commandline_option_provided("-r", "--only-right")) {
		if (khepera3_motor_initialize(&(khepera3.motor_left))) {
			printf("Left motor initialized.\n");
		} else {
			printf("Left motor: initialization failed.\n");
		}
	}

	// Initialize right motor
	if (! commandline_option_provided("-l", "--only-left")) {
		if (khepera3_motor_initialize(&(khepera3.motor_right))) {
			printf("Right motor initialized.\n");
		} else {
			printf("Right motor: initialization failed.\n");
		}
	}

	return 0;
}
