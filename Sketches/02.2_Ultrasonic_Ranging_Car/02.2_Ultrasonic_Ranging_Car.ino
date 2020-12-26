/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Description : Ultrasonic Car.
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"

#define OBSTACLE_DISTANCE      40
#define OBSTACLE_DISTANCE_LOW  20
int distance[4];     //Storage of ultrasonic data

void setup() {
  Ultrasonic_Setup();//Initialize the ultrasonic module
  PCA9685_Setup();   //PCA9685 chip initializes
}

void loop()
{
  get_distance(1);//Get multiple Angle distance data
  if (distance[1] > OBSTACLE_DISTANCE)//There is no obstacle within 40cm of the front of the car
  {
    if (distance[0] >= distance[2] && distance[2] < OBSTACLE_DISTANCE_LOW)    //There is an obstacle on the right
      Motor_Move(-1000, -1000, 2000, 2000);                                   
    else if (distance[0] < distance[2] && distance[0] < OBSTACLE_DISTANCE_LOW)//There is an obstacle on the left
      Motor_Move(2000, 2000, -1000, -1000);                                  
    else                                                                      //There are no obstacles around
      Motor_Move(1000, 1000, 1000, 1000);                                                                         
    delay(20);
  }

  else if (distance[1] < OBSTACLE_DISTANCE)  //There is an obstacle ahead
  {
    Motor_Move(0, 0, 0, 0);                  //Stop the car to judge the situation
    get_distance(2);
    if (distance[1] < OBSTACLE_DISTANCE_LOW) //The car got too close to the obstacle
      Motor_Move(-1000, -1000, -1000, -1000);
    else if (distance[0] < distance[2])      //There is also an obstacle on the left
      Motor_Move(2000, 2000, -2000, -2000);
    else if (distance[0] > distance[2])      //There is also an obstacle on the right
      Motor_Move(-2000, -2000, 2000, 2000);
    delay(200);
  }
}

//Get distance values for different angles
void get_distance(int car_mode)
{
  int add_angle=30;
  if(car_mode==1)
    add_angle=0;
  else
    add_angle=30;
    
  Servo_2_Angle(90);  
  Servo_1_Angle(120+add_angle);
  delay(100);
  distance[0] = Get_Sonar();//Get the data on the left

  Servo_1_Angle(90);
  delay(100);
  distance[1] = Get_Sonar();//Get the data in the middle

  Servo_1_Angle(60-add_angle);
  delay(100);
  distance[2] = Get_Sonar();//Get the data on the right

  Servo_1_Angle(90);
  delay(100);
  distance[1] = Get_Sonar();//Get the data in the middle
}
