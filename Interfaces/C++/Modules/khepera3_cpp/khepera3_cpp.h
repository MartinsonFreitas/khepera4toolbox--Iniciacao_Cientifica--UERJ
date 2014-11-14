#ifndef KHEPERA3_CPP_H
#define KHEPERA3_CPP_H

extern "C" {
#include "khepera3.h"
}

class Khepera {

public:
	const static int NR_SENSORS = cKhepera3SensorsInfrared_Count;
private:
	const static float steepness = 25;
	const static float thresholdValue = 250;
	const static int def_maxSpeed = 40000;
	sKhepera3SensorsInfrared irData;
	sKhepera3SensorsInfrared irAmbientData;
	int maxSpeed;

	static Khepera* khepera;

	// private constructor, singleton
	Khepera();
	void updateIR();
	void updateAmbientIR();
	static float normalizeIR(float raw);
public:
	// singleton getter
	static Khepera* getKhepera();
	// return sensor value, sensors go from 0 to NR_SENSORS -1
	float getIR(int pos);
	float getRawIR(int pos);
	void setNormalizedSpeeds(float left, float right);
	void setLeftSpeed(float speed);
	void setRightSpeed(float speed);
	int* getAllRawIR();
	int* getAllRawAmbientIR();
	void setMaxSpeed(int speed);
};


#endif
