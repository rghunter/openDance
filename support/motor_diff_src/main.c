/*!
 * (c) 2006-2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include "khepera3.h"
#include "commandline.h"
#include "measurement.h"
#include <stdio.h>
#include <stdlib.h>


// Prints the help text.
void help() {
	printf("Used to make arcs\n");
	printf("\n");
	printf("Usage:\n");
	printf("  motor_diff L R 	 L is the left speed R is the Right Speed\n");
	printf("  motor_diff -d [N]      Sets the distance of trave\n");
	printf("\n");
}

// Takes one measurement

// Main program.
int main(int argc, char *argv[]) {
	// Command line parsing
	commandline_init();
	measurement_init();
	measurement_commandline_prepare();
	commandline_parse(argc, argv);

	//initializing
	khepera3_init();


	khepera3_motor_initialize(&(khepera3.motor_left));
	khepera3_motor_initialize(&(khepera3.motor_right));
	khepera3_motor_start(&(khepera3.motor_left));
	khepera3_motor_start(&(khepera3.motor_right));

	// Help
	if (commandline_option_provided("-h", "--help")) {
		help();
		exit(1);
	}
	int sp_left = commandline_argument_int(0, 1000);
	int sp_right = commandline_argument_int(1, 1000);
	int dist = commandline_option_value_int("-d","--dist",1000);

	printf("Left Speed: %i Right Speed: %i Distance: %i\n",sp_left,
			sp_right,dist);


	khepera3_motor_set_current_position(&(khepera3.motor_left),0);
	khepera3_motor_set_current_position(&(khepera3.motor_right),0);
	printf("test\n");
	khepera3_motor_set_speed(&(khepera3.motor_left),sp_left);
	khepera3_motor_set_speed(&(khepera3.motor_right),sp_right);
	printf("test2\n");
	int curr = 0;

	// Take continuous measures
	for(;;)
	{
		khepera3_motor_get_current_position(&(khepera3.motor_left));
		khepera3_motor_get_current_position(&(khepera3.motor_right));
		if(dist <= (curr=(abs(sp_left)>abs(sp_right) ?
				       	abs(khepera3.motor_right.current_position) : abs(khepera3.motor_left.current_position))))
		{
			khepera3_motor_set_speed(&(khepera3.motor_left),0);
			khepera3_motor_set_speed(&(khepera3.motor_right),0);
			return 0;
		}
		printf("Current Distance: %i\n",curr);
	}




	return 0;
}
