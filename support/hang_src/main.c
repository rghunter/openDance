/*!
 * (c) 2006-2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include "commandline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

// Algorithm parameters and results
struct sAlgorithm {
	struct {
		int port;
		char hostname[1024];
		int binary;
		int verbosity;
	} configuration;
	struct {
		char buffer[1024];
		int len;
	} message;
};

// Algorithm instance
struct sAlgorithm algorithm;

// Prints the help text.
void help() {
	printf("Sends a UDP packet to another computer/robot. By default, port 3000 is used.\n");
	printf("\n");
	printf("Usage: udp_sender [OPTIONS] HOST[:PORT] MESSAGE\n");
	printf("\n");
	printf("Options:\n");
	printf("  -b --binary           Switches to binary message interpretation (see examples)\n");
	printf("  -v --verbosity V      Sets the verbosity level (0=quiet, 1=default, 2=verbose, 3=very verbose, ...)\n");
	printf("\n");
	printf("Examples:\n");
	printf("  Send a text message to port  robot 203 (192.168.141.203):\n");
	printf("    udp_sender 192.168.141.203 \"Hello world\"\n");
	printf("  Send a binary message (ABC) to same robot:\n");
	printf("    udp_sender -b 192.168.141.203 65 66 67\n");
	printf("  The same, but with hex notation:\n");
	printf("    udp_sender -b 192.168.141.203 0x41 0x42 0x43\n");
	printf("\n");
}

void parse_hostname_port() {
	char *port_str;

	// Check if there is a ':' sign in the hostname
	port_str = strchr(algorithm.configuration.hostname, ':');
	if (! port_str) {
		return;
	}

	// Terminate the hostname string there
	*port_str = 0;

	// Convert the rest of the string to a number
	port_str++;
	algorithm.configuration.port = strtol(port_str, 0, 0);
	if ((algorithm.configuration.port < 1) || (algorithm.configuration.port > 65535)) {
		printf("Invalid port: %s (interpreted as %d)\n", port_str, algorithm.configuration.port);
		exit(2);
	}
}

void parse_message() {
	strcat(algorithm.message.buffer, "continue");
	algorithm.message.len = 8;

}

// Initializes the algorithm.
void algorithm_init() {
	// Read command line options
	algorithm.configuration.binary = commandline_option_provided("-b", "--binary");
	algorithm.configuration.verbosity = commandline_option_value_int("-v", "--verbosity", 1);

	// Check if the user provided enough arguments
	if (commandline_argument_count() < 1) {
		help();
		exit(1);
	}

	// Get hostname and port (first argument)
	algorithm.configuration.port = commandline_option_value_int("-p", "--port", 3000);
	strncpy(algorithm.configuration.hostname, commandline_argument(0, 0), 1023);
	parse_hostname_port();

	// Parse the message
	parse_message();
}

// Runs the algorithm by calling the appropriate state function.
void algorithm_run() {
	int sock;
	struct sockaddr_in address;
	struct sockaddr_in sender;
	struct sockaddr_in recieve;
	struct hostent *host;
	int res, bindSock;
	int sender_len;
	int recv_len;
	char recv_buffer[1024];

	// Initialize the address
	address.sin_family = AF_INET;
	recieve.sin_family = AF_INET;
	address.sin_port = htons(algorithm.configuration.port);
	recieve.sin_port = htons(algorithm.configuration.port);
	recieve.sin_addr.s_addr = htonl(INADDR_ANY);

	// Hostname resolution
	host = gethostbyname(algorithm.configuration.hostname);
	if (! host) {
		printf("Unable to resolve hostname %s\n", algorithm.configuration.hostname);
		exit(2);
	}
	address.sin_addr.s_addr = *(unsigned long*)host->h_addr;

	// Create datagram socket and send the datagram
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	bindSock = bind(sock, (struct sockaddr *)&recieve, sizeof(recieve));
	if(bindSock)
	{
		printf("COULD NOT BIND\n");
	}
	int exit = 0;

	for(;;)
	{
		memset(recv_buffer,0,sizeof(recv_buffer));
		int i;
		for(i = 0; i<3; i++)
		{
			res = sendto(sock, algorithm.message.buffer, algorithm.message.len, 0, (struct sockaddr *) & address, sizeof(struct sockaddr_in));
			if (res > 0) {
				printf("UDP datagram (%d bytes) sent!\n", res);
			} else {
				printf("Error (%d) while sending UDP datagram!\n", res);
			}
			usleep(100);
		}
		if(exit)
			return 0;
		recv_len = recvfrom(sock, recv_buffer, 1023, 0, (struct sockaddr *)&sender, &sender_len);
		printf("curr buffer: %s\n",recv_buffer);

		if(strcmp(recv_buffer,"continue") == 0)
			exit = 1;
		usleep(1000);
	}
}

// Main program.
int main(int argc, char *argv[]) {
	// Command line parsing
	commandline_init();
	commandline_option_register("-b", "--binary", cCommandLine_Option);
	commandline_parse(argc, argv);

	// Help
	if (commandline_option_provided("-h", "--help")) {
		help();
		exit(1);
	}

	// Initialization
	algorithm_init();

	// Run
	algorithm_run();
	return 0;
}
