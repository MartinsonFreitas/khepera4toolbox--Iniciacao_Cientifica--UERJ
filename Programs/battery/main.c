/*!
 * (c) 2006-2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#include "khepera3.h"
#include "commandline.h"
#include "measurement.h"
#include <stdio.h>
#include <stdlib.h>

// Buffers
struct sMeasurement {
	int sample_number;
	struct sKhepera3Battery battery;
};

#define LOG_SIZE 1024
struct sMeasurement log_buffer[LOG_SIZE];
unsigned int sample_number = 0;

// Prints the help text.
void help() {
	printf("Measures the battery voltage.\n");
	printf("\n");
	printf("Usage:\n");
	printf("  battery             Measures one set of values\n");
	printf("  battery -r [N]      Measures N sets of values (N=infinity if not provided)\n");
	printf("  battery -r N -f     Measures N sets of values and keeps them in memory before printing (for fast acquisition)\n");
	printf("  battery -r N -F     Measures N sets of values but does not print them\n");
	printf("  battery -w US       Waits US us after each measurement\n");
	printf("\n");
	printf("Output format:\n");
	printf("  $BATTERY, sample_number,\n");
	printf("            voltage [mV],\n");
	printf("            current [mA],\n");
	printf("            current_average [mA],\n");
	printf("            capacity_remaining_absolute [mAh],\n");
	printf("            capacity_remaining_relative [%%],\n");
	printf("            temperature [deg C]\n");
	printf("\n");
}

// Takes one measurement
void measurement_take(int i) {
	khepera3_battery();
	log_buffer[i].sample_number = sample_number;
	log_buffer[i].battery = khepera3.battery;
	sample_number++;
}

// Prints one measurement
void measurement_print(int i) {
	printf("$BATTERY,%d,%#x,%u,%d,%d,%u,%u,%2.1f\n",
	       log_buffer[i].sample_number,
               log_buffer[i].battery.status,
	       (log_buffer[i].battery.voltage),
	       (log_buffer[i].battery.current),
	       (log_buffer[i].battery.current_average),
	       (log_buffer[i].battery.capacity_remaining_absolute),
	       (log_buffer[i].battery.capacity_remaining_relative),
	       (log_buffer[i].battery.temperature)
	      );
}

// Main program.
int main(int argc, char *argv[]) {
	// General initialization
	khepera3_init();
	measurement_init();

	// Command line parsing
	commandline_init();
	measurement_commandline_prepare();
	commandline_parse(argc, argv);

	// Help
	if (commandline_option_provided("-h", "--help")) {
		help();
		exit(1);
	}

	// Take continuous measurements
	measurement_configuration.log_size = LOG_SIZE;
	measurement_configuration.hook_measure = &measurement_take;
	measurement_configuration.hook_print = &measurement_print;
	measurement_start();

	return 0;
}

