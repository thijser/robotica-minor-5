/* 
 * rosserial Ultrasound Example
 * 
 * This example is for the Maxbotix Ultrasound rangers.
 */

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#define trigPin 13
#define echoPin 12
#define led 11
#define led2 10
#define speedOfSound 3.4;
ros::NodeHandle  nh;
sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);
char frameid[] = "/ultrasound";

float getRange_Ultrasound(int pin_num){
  int val = 0;
 val += analogRead(pin_num);
  float range =  val;
  return range ;   // (0.0124023437 /4) ; //cvt to meters
}

void setup(){
  nh.initNode();
  nh.advertise(pub_range);
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  
  range_msg.min_range = 40.0;
  range_msg.max_range = 10000;

}


long range_time;

void loop()
{
  
  //publish the adc value every 50 milliseconds
  //since it takes that long for the sensor to stablize
     long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2);
    range_msg.range = distance/speedOfSound;
    range_msg.header.stamp = nh.now();
    range_time =  millis() + 50;
  pub_range.publish(&range_msg);
  
  nh.spinOnce();
}
