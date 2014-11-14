#include "khepera3_ultrasound.h"
#include "khepera3.h"

char ultrasound_name[5] = "us0";
DeviceTag ultrasound[5]; 				//!< Remember DeviceTag equals to const char
const char *cKhepera3SensorsUltrasound[5] = {"left", "frontleft", "front", "frontright", "right"};

void khepera3_ultrasound_init() {
	int i;

	// Ultrasound assigment
	for (i = cKhepera3SensorsUltrasound_Begin; i < cKhepera3SensorsUltrasound_End; i++) {
		ultrasound[i] = robot_get_device(ultrasound_name);
		distance_sensor_enable(ultrasound[i], CONTROL_STEP);
		if ((i + 1) >= 10) {
			ultrasound_name[2] = '1';
			ultrasound_name[3]++;
			if ((i + 1) == 10) {
				ultrasound_name[3] = '0';
				ultrasound_name[4] = (char) '\0';
			}

		} else {
			ultrasound_name[2]++;
		}

	}

	khepera3.ultrasound.distance_per_increment = 0.01;
	ultrasound_structure_init();
}

void ultrasound_structure_init() {
	int i, j;

	for (i = cKhepera3SensorsUltrasound_Begin; i < cKhepera3SensorsUltrasound_End; i++) {
		for (j = 0; j < 10; j++) {
			khepera3.ultrasound.sensor[i].distance[j] = 0;
			khepera3.ultrasound.sensor[i].amplitude[j] = 0;
			khepera3.ultrasound.sensor[i].distance[j] = 0;
			khepera3.ultrasound.sensor[i].timestamp[j] = 0;
		}

		khepera3.ultrasound.sensor[i].amplitude[0] = AMPLITUDE;
	}
}

int khepera3_ultrasound(enum eKhepera3SensorsUltrasound sensor) {
	khepera3.ultrasound.sensor[sensor].distance[0] = (int)distance_sensor_get_value(ultrasound[sensor]);
	khepera3.ultrasound.sensor[sensor].timestamp[0] = robot_get_time();
}

enum eKhepera3SensorsUltrasound khepera3_ultrasound_getsensorbyname(const char *name, enum eKhepera3SensorsUltrasound defaultvalue) {
	int i;

	if (name == 0) {
		return defaultvalue;
	}

	// By name
	for (i = 0; i < 5; i++) {
		if (strncmp(name, cKhepera3SensorsUltrasound[i]) == 0) {
			return i;
		}
	}

	// Nothing
	return defaultvalue;
}

enum eKhepera3SensorsUltrasoundBit khepera3_ultrasound_getsensorbitbysensor(enum eKhepera3SensorsUltrasound sensor) {
	return (1 << sensor);
}
