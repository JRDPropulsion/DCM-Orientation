# DCM-Orientation

## INTRODUCTION
DCM or direction cosine matrix is a method used to transform one reference frame into another. Although other methods of determining orientation such as quaternions have proven to be a popular option, DCM still stand as a viable option for many and are far easier to understand. This library uses the DCM algorithm to convert raw gyroscopic rates in rad/s to the body frame orientation.

## USER GUIDE
In order to use this library effectively, you must obtain the raw gyroscopic rates in rad/s and pass them to the update function. A common mistake is to pass degrees instead of radians so make sure the gyro rates are in radians!

## EXAMPLES
Currently, there is an example for the BMI088 IMU however, I plan on releasing examples for other sensors such as the BNO055 and MPU6050 in the future. Note that the gyro bias calculation in each example is not strictly necessary but helps a lot to combat gyro drift.
