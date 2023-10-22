/*
  Direction cosine matrix example BMI088
  By: Aryan Kapoor
  Last updated on: 5/21/23
*/


/*
  Importing libraries
*/
#include <Arduino.h>
#include <Wire.h>
#include <BMI088.h>
#include <orientation.h>

/*
  Initialization and definitions
*/
Bmi088Gyro gyro(Wire,0x68); 
orientation ori;

// Global variable definitions
double gyro_x, gyro_y, gyro_z, gyro_bais_x, gyro_bais_y, gyro_bais_z,
       gyro_x_cal, gyro_y_cal, gyro_z_cal, gyro_x_offset, gyro_y_offset, 
       gyro_z_offset, body_ori_x, body_ori_y, body_ori_z;

int state = 0;


/*
  Update sensors
*/
void sensors_update() 
{
  gyro.readSensor();

  gyro_x = gyro.getGyroX_rads();
  gyro_y = gyro.getGyroY_rads();
  gyro_z = gyro.getGyroZ_rads();

  // Calibrated gyro measurements
  gyro_x_cal = (gyro.getGyroX_rads() - gyro_bais_x) + gyro_x_offset; // Calibrated x gyro measurement
  gyro_y_cal = (gyro.getGyroY_rads() - gyro_bais_y) + gyro_y_offset; // Calibrated y gyro measurement
  gyro_z_cal = (gyro.getGyroZ_rads() - gyro_bais_z) + gyro_z_offset; // Calibrated z gyro measurement
}


/*
  Finds the gyro bias by taking the average of a set of gyro measurements
*/
void gyro_bias() 
{
  // Initialize local gyro sums as 0
  double gyro_x_sum = 0;
  double gyro_y_sum = 0;
  double gyro_z_sum = 0;

  int count = 0;
  int sample_count = 1000; // Total number of samples collected

  while (count < sample_count) 
  {
    sensors_update();

    // Intergrate gyro measurements to get summation
    gyro_x_sum += gyro_x;
    gyro_y_sum += gyro_y;
    gyro_z_sum += gyro_z;

    delay(20);
    count += 1;
  }


  /*
    All functions that happen right after the gyro biases are calculated
  */
  if (count == sample_count) 
  {
    // Get average of summation
    gyro_bais_x = gyro_x_sum / sample_count;
    gyro_bais_y = gyro_y_sum / sample_count;
    gyro_bais_z = gyro_z_sum / sample_count;

    sensors_update();

    // Calibrated gyro measurements without orientation offset
    gyro_x_cal = (gyro_x - gyro_bais_x);
    gyro_y_cal = (gyro_y - gyro_bais_y);
    gyro_z_cal = (gyro_z - gyro_bais_z);

    gyro_x_offset = -(gyro_x_cal);
    gyro_y_offset = -(gyro_y_cal);
    gyro_z_offset = -(gyro_z_cal);

    state++;
  }
}


/*
  Setup function
*/
void setup() 
{
  Serial.begin(9600);
  Wire.begin();  
  gyro.begin();
  ori.begin();

  // Initialize sensors
  gyro.setOdr(Bmi088Gyro::ODR_1000HZ_BW_116HZ); // Set gyro update rate to 1000Hz 
  gyro.setRange(Bmi088Gyro::RANGE_500DPS); // Set the gyro range to 500 degrees per second
}


/*
  Loop function
*/
void loop()
{
  /*
    If statements to switch states and call certain functions that run only if the state matches
  */
  if (state == 0) 
  {
    gyro_bias();
  }

  if (state == 1)
  {
    sensors_update(); // Update sensors
    ori.update(gyro_z_cal, gyro_y_cal, gyro_x_cal, &body_ori_x, &body_ori_y, &body_ori_z);

    Serial.print("X: ");
    Serial.print(body_ori_x);
    Serial.print("      Y: ");
    Serial.println(body_ori_y);
    delay(10); // Use delay only for testing, remove afterwards
  }

}
