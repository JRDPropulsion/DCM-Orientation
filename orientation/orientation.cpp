/*
  orientation.h - Directional cosine matrix library
  Created by Aryan Kapoor, 5/21/23.
  Released into the public domain.
*/

// Importing libraries
#include <Arduino.h>
#include <orientation.h>


void orientation::begin()
{
  prev_interval = millis();
}


/*
  Function to compute the orientation using DCM matrix
*/
float orientation::update(float gyro_x, float gyro_y, float gyro_z, double *body_ori_x, double *body_ori_y, double *body_ori_z)
{

  interval = (millis() - prev_interval) * 0.001;

  // Set the measurements from the previous time step as the current
  prev_gyro_tilt_x = gyro_tilt_x;
  prev_gyro_tilt_y = gyro_tilt_y;
  prev_gyro_tilt_z = gyro_tilt_z;

  // Convert rad/s to tilt angle in rad and find the difference in the measurements
  gyro_tilt_x += (gyro_x * interval);
  gyro_tilt_y += (gyro_y * interval);
  gyro_tilt_z += (gyro_z * interval);
  
  prev_body_ori_x = ori_x;
  prev_body_ori_y = ori_y;
  prev_body_ori_z = ori_z;

  prev_interval = millis();

  x = (gyro_tilt_x - prev_gyro_tilt_x);
  y = (gyro_tilt_y - prev_gyro_tilt_y);
  z = (gyro_tilt_z - prev_gyro_tilt_z);

  // Define row 1 of DCM matrix
  A11 = cos(z) * cos(y);
  A21 = (cos(z) * sin(y) * sin(x)) - (sin(z) * cos(x));
  A31 = (cos(z) * sin(y)) * cos(x) + (sin(z) * sin(x));

  // Define row 2 of DCM matrix
  A12 = sin(z) * cos(y);
  A22 = (sin(z) * sin(y) * sin(x)) + (cos(z) * cos(x));
  A32 = (sin(z) * sin(y) * cos(x)) - (cos(z) * sin(x));

  // Define row 3 of DCM matrix
  A13 = -sin(y);
  A23 = cos(y) * sin(x);
  A33 = cos(y) * cos(x);

  ori_x = (A11 * prev_body_ori_x) + (A21 * prev_body_ori_y) + (A31 * prev_body_ori_z);
  ori_y = (A12 * prev_body_ori_x) + (A22 * prev_body_ori_y) + (A32 * prev_body_ori_z);
  ori_z = (A13 * prev_body_ori_x) + (A23 * prev_body_ori_y) + (A33 * prev_body_ori_z);

  // Calculate body orientation in degrees
  *body_ori_x = asin(ori_x) * RAD_TO_DEG;
  *body_ori_y = asin(ori_y) * RAD_TO_DEG;
  *body_ori_z = asin(ori_z) * RAD_TO_DEG;

  return 0;
}
