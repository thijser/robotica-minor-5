/* 
 * rosserial Ultrasound Example
 * 
 * This example is for the Maxbotix Ultrasound rangers.
 */

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

#define sensNumber 4
#define firstTrigPin 2
#define firstEchoPin 8


ros::NodeHandle  nh;
sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);
char frameid[] = "/ultrasound";

 int trigPin=firstTrigPin;
 int echoPin=firstEchoPin;

void increasepins(){
  trigPin++;
  echoPin++;
  if(trigPin>firstTrigPin+sensNumber){
    trigPin=firstTrigPin;}
  if(echoPin>firstEchoPin+sensNumber){
    echoPin=firstEchoPin;}
}

void setup(){
  nh.initNode();
  nh.advertise(pub_range);
  for(int i=0;i<sensNumber;i++){
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  increasepins();  
}
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  
  range_msg.min_range = 0.0;
  range_msg.max_range = 6.47;

}


long range_time;

void loop()
{
   increasepins();
  //publish the adc value every 50 milliseconds
  //since it takes that long for the sensor to stablize
     long duration, distance;
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  distance = pulseIn(echoPin, HIGH,100000)/ 58.2; //change timeout value, increase for more range decrease for higher sample rate 
 

    range_msg.range = distance;
    range_msg.header.stamp = nh.now();
    range_time =  millis() + 50;
    range_msg.field_of_view = trigPin; 
    range_msg.min_range = echoPin;

  pub_range.publish(&range_msg);
  
  nh.spinOnce();
}
