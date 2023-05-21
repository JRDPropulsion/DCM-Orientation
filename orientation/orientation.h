/*
  orientation.h - Direction cosine matrix library
  Created by Aryan Kapoor, 5/21/23.
  Released into the public domain.
*/

#ifndef ORIENTATION_H
#define ORIENTATION_H
#include <Arduino.h>

class orientation
{
    public:
        void begin();
        float update(float gyro_x, float gyro_y, float gyro_z, double *body_ori_x, double *body_ori_y, double *body_ori_z);

        private:

        float prev_gyro_tilt_x = 0;
        float prev_gyro_tilt_y = 0;
        float prev_gyro_tilt_z = 0;
        float prev_body_ori_x;
        float prev_body_ori_y;
        float prev_body_ori_z;
        float ori_x = 0;
        float ori_y = 0;
        float ori_z = 1;

        float gyro_tilt_x, gyro_tilt_y, gyro_tilt_z, 
              body_ori_x, body_ori_y, body_ori_z, x, y, z, 
              A11, A12, A13, A21, A22, A23, A31, A32, A33;

        float interval;
        long prev_interval;
};

#endif
