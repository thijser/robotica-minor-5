/* 
 * rosserial Ultrasound Example
 * 
 * This example is for the Maxbotix Ultrasound rangers.
 */

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#define trigPin1 2
#define trigPin2 3
#define trigPin3 4
#define trigPin4 5

#define echoPin1 8
#define echoPin2 9
#define echoPin3 10
#define echoPin4 11

#define led 11
#define led2 10

ros::NodeHandle  nh;
sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);
char frameid[] = "/ultrasound";


void setup(){
  nh.initNode();
  nh.advertise(pub_range);
   pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
   pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
   pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
   pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);  
  
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  
  range_msg.min_range = 0.0;
  range_msg.max_range = 6.47;

}


long range_time;

void loop()
{
  
  //publish the adc value every 50 milliseconds
  //since it takes that long for the sensor to stablize
     long duration, distance;
  digitalWrite(trigPin1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin1, HIGH);

  digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  
    digitalWrite(trigPin3, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin3, HIGH);
  
    digitalWrite(trigPin4, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin4, HIGH);
  
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);
  
  long duration1 = pulseIn(echoPin1, HIGH);
  long duration2 = pulseIn(echoPin2, HIGH);
  long duration3 = pulseIn(echoPin3, HIGH);
  long duration4 = pulseIn(echoPin4, HIGH);
  long distance1 = (duration/2) / 29.1;
  long distance2 = (duration/2) / 29.1;
  long distance3 = (duration/2) / 29.1;
  long distance4 = (duration/2) / 29.1;
  
  range_msg.range = distance1;
  range_msg.header.stamp = nh.now();
  range_time =  millis() + 50;
  range_msg.field_of_view = 1;  
  pub_range.publish(&range_msg);

  range_msg.range = distance2;
  range_msg.header.stamp = nh.now();
  range_time =  millis() + 50;
  range_msg.field_of_view = 2;
  pub_range.publish(&range_msg);

  range_msg.range = distance3;
  range_msg.header.stamp = nh.now();
  range_time =  millis() + 50;
  range_msg.field_of_view = 3;
  pub_range.publish(&range_msg);

  range_msg.range = distance4;
  range_msg.header.stamp = nh.now();
  range_time =  millis() + 50;
    range_msg.field_of_view = 4;
  pub_range.publish(&range_msg);
  nh.spinOnce();
}
