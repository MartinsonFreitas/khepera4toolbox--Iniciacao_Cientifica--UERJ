/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 * Modifications for Webots: Iñaki Navarro
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "khepera4.h"
#include "odometry_track.h"

#define WHEEL_RADIUS 0.0212
#define AXLE_LENGTH 0.0893
#define ENCODER_RESOLUTION 440

void odometry_track_init(int time_ms) {
	differential_wheels_enable_encoders((unsigned short) time_ms);
}

int odometry_track_start(struct sOdometryTrack * ot) {
	khepera4_drive_get_current_position();
	return odometry_track_start_pos(ot, khepera4.motor_left.current_position, khepera4.motor_right.current_position);
}

int odometry_track_start_pos(struct sOdometryTrack * ot, int pos_left, int pos_right) {
	ot->result.x = 0;
	ot->result.y = 0;
	ot->result.theta = 0;

	ot->state.pos_left_prev = pos_left;
	ot->state.pos_right_prev = pos_right;

	// Odometry: default values
	ot->configuration.is_default = 1;
	ot->configuration.wheel_distance = AXLE_LENGTH;
	ot->configuration.wheel_conversion_left = WHEEL_RADIUS / ENCODER_RESOLUTION;
	ot->configuration.wheel_conversion_right = WHEEL_RADIUS / ENCODER_RESOLUTION;
	return 1;
}

void odometry_track_step(struct sOdometryTrack * ot) {
	khepera4_drive_get_current_position();
	odometry_track_step_pos(ot, khepera4.motor_left.current_position, khepera4.motor_right.current_position);
}

void odometry_track_step_pos(struct sOdometryTrack * ot, int pos_left, int pos_right) {
	long delta_pos_left, delta_pos_right;
	float delta_left, delta_right, delta_theta, theta2;
	float delta_x, delta_y;

	delta_pos_left = pos_left - ot->state.pos_left_prev;
	delta_pos_right = pos_right - ot->state.pos_right_prev;
	delta_left = delta_pos_left * ot->configuration.wheel_conversion_left;
	delta_right = delta_pos_right * ot->configuration.wheel_conversion_right;
	delta_theta = (delta_left - delta_right) / ot->configuration.wheel_distance;
	theta2 = ot->result.theta + delta_theta * 0.5;
	delta_x = (delta_left + delta_right) * 0.5 * cosf(theta2);
	delta_y = (delta_left + delta_right) * 0.5 * sinf(theta2);

	ot->result.x += delta_x;
	ot->result.y += delta_y;
	ot->result.theta += delta_theta;
	ot->result.theta = ot->result.theta;
	ot->state.pos_left_prev = pos_left;
	ot->state.pos_right_prev = pos_right;
}
