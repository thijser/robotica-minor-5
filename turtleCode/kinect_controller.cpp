#include <world_perception/KinectController.h>

KinectController::KinectController():
  follow(false),
  target(-1),
  tolerance(0.01),
  angle_tolerance(0.003),
  encoder_trust(0),
  base_width(0.3),
  dist_treshold(0.6),
  gap_treshold(1)
{
  nh_.param("tolerance", tolerance, tolerance);
  nh_.param("angle_tolerance", angle_tolerance, angle_tolerance);
  nh_.param("encoder_trust", encoder_trust, encoder_trust);
  nh_.param("base_width", base_width, base_width);
  nh_.param("dist_treshold", dist_treshold, dist_treshold);
  shutdown_command = nh_.subscribe<std_msgs::Bool>("shutdown", 1, &KinectController::shutdown, this);
  laserscan_sub = nh_.subscribe<sensor_msgs::LaserScan>("scan", 1, &KinectController::depthLaserScanBack, this);
  gap_command = nh_.subscribe<std_msgs::Bool>("gap_command", 1, &KinectController::findGapCallback, this);
  dist_sub = nh_.subscribe<world_perception::dist_command>("dist_command", 1, &KinectController::distCallback, this);
  encoder_sub = nh_.subscribe<world_perception::pos_feedback>("robot_actuator_position", 1, &KinectController::encoderCallback, this);
  stop_command = nh_.subscribe<std_msgs::Bool>("stop", 1, &KinectController::stop, this);
  obstacle = nh_.advertise<std_msgs::Bool>("clear_path", 1);
  speed_cmd = nh_.advertise<world_perception::command_speed>("robot_actuator_command_speed", 1);
  angle_pub = nh_.advertise<std_msgs::Float32>("platform_angle", 1);
  message_pub = nh_.advertise<std_msgs::String>("console_response", 1);
  dleft_pub = nh_.advertise<std_msgs::Float32>("dleft", 1);
  dright_pub = nh_.advertise<std_msgs::Float32>("dright", 1);
  dist_pub = nh_.advertise<world_perception::dist_command>("dist_command", 1);
  prev_scan_data_init = false;
  go_gap = false;
  set_stop = false;

  //Publisher for moving through gaps
  position_pub = nh_.advertise<world_perception::command_pos>("robot_actuator_command_pos", 1);
}

void KinectController::shutdown(const std_msgs::Bool::ConstPtr& b){
  ros::shutdown();
}

void KinectController::stop(const std_msgs::Bool::ConstPtr& b){
  target = -1;
  go_gap = false; 
  set_stop = true;
}

void KinectController::depthLaserScanBack(const sensor_msgs::LaserScan::ConstPtr& scan){
  scan_data = scan->ranges;
  angle_min = scan->angle_min;
  angle_max = scan->angle_max;
  angle_increment = scan->angle_increment;
  range_min = scan->range_min;
  range_max = scan->range_max;
  data_size = (int) scan_data.size();
  if(!prev_scan_data_init){
    prev_scan_data = scan_data;
    prev_scan_data_init = true;
  }
  
  std::reverse(scan_data.begin(), scan_data.end());
  parseKinectData();
  publishAngle();
  pathClear();
  controlDistance();
  differentiate();
  goThroughGap();
  prev_scan_data = scan_data;
}

void KinectController::parseKinectData(){
  obj_width_idx = (int) (floor(atan( (0.5 * base_width) / dist_treshold) / angle_increment));
  i_mid = (int) data_size / 2;
  int i_tmp = 0;
  while(std::isnan(scan_data[i_tmp])){
    i_tmp++;
  }

  for(int i = 0; i < i_tmp; i++){
    scan_data[i] = scan_data[i_tmp];
  }
  for(int i = i_tmp; i < data_size; i++){
    if(std::isnan(scan_data[i])){
      scan_data[i] = scan_data[i - 1];
    }
  }
  //printf("angle_width: %f, base_width: %f, depth_mid: %f\n", angle_width, base_width, scan_data[i_mid]);
  if(target == -1){
  angle_width = atan( (0.5 * base_width) / scan_data[i_mid]);
  angle_width_idx = (int) (floor(angle_width / angle_increment));
}
  kin_d_left = scan_data[i_mid - angle_width_idx];
  kin_d_right = scan_data[i_mid + angle_width_idx];
}

/**
Checkt of ter breedte van het platform plus een beetje (base width) het pad vrij is tot op dist_treshold diep.
*/
void KinectController::pathClear(){
  clear_path = true;
  for(int i = i_mid - obj_width_idx; i < i_mid + obj_width_idx; i++){
    if((std::isnan(scan_data[i]) && !std::isnan(prev_scan_data[i]) && prev_scan_data[i] < (range_max / 2)) || scan_data[i] < dist_treshold){
      scan_data[i] = prev_scan_data[i];
      clear_path = false;
      break;
    }
  }
  std_msgs::Bool msg;
  msg.data = clear_path;
  obstacle.publish(msg);
}

void KinectController::publishAngle(){
  float tangle = ((d_left - d_right) * cos(angle_width)) / (sin(angle_width) * (d_left + d_right));
  //printf("target: %f, Angle width: %f, d_left: %f, d_right: %f , clear path: %i \n base_angle: %f \n", target, angle_width, d_left, d_right, clear_path, base_angle );
  base_angle = atan(tangle);
  std_msgs::Float32 angle_msg;
  angle_msg.data = base_angle * (360 / (2 * M_PI));
  angle_pub.publish(angle_msg);
}

void KinectController::controlDistance(){
  
    



  if(!clear_path || set_stop){
    
    d_left = kin_d_left;
    d_right = kin_d_right;
    world_perception::command_speed msg;
    msg.omega = 0;
    msg.v = 0;
    speed_cmd.publish(msg);
    set_stop = false;
  } else if(target != -1){
    float left_enc_distance = left_enc_pos - prev_left_enc_pos;
    float right_enc_distance = right_enc_pos - prev_right_enc_pos;
    float left_kin_distance = prev_kin_d_left - kin_d_left;
    float right_kin_distance = prev_kin_d_right - kin_d_right;
    
    prev_kin_d_left = kin_d_left;
    prev_kin_d_right = kin_d_right;


    d_left = prev_d_left - ((left_enc_distance * encoder_trust) + ((1 - encoder_trust) * left_kin_distance));
    d_right =  prev_d_right - ((right_enc_distance * encoder_trust) + ((1 - encoder_trust) * right_kin_distance));

    std_msgs::Float32 dleft_msg;
    std_msgs::Float32 dright_msg;
    dleft_msg.data = kin_d_left;
    dright_msg.data = kin_d_right;
    dleft_pub.publish(dleft_msg);
    dright_pub.publish(dright_msg);

    //ROS_INFO("distance [%f, %f], angle %f,  target %f", kin_d_left, kin_d_right, base_angle, target);
    world_perception::command_speed msg;
    if(std::abs(base_angle) > angle_tolerance ){
      msg.omega = base_angle * 15;
    } else {
      msg.omega = 0;
    }
    
    msg.v = (fmin(kin_d_left, kin_d_right) - target) * 15;
    speed_cmd.publish(msg); 

    printf("v: %f, omega: %f\n", msg.v, msg.omega);
    
    if(std::abs(d_left - d_right) < tolerance && std::abs(fmin(d_left, d_right) - target) < tolerance && base_angle < angle_tolerance && !follow){
      target = -1;
      set_stop = true;
    }
  }
  prev_d_left = d_left;
  prev_d_right = d_right;
}

void KinectController::distCallback(const world_perception::dist_command::ConstPtr& cmd){
  if(cmd->controller == 1){
    target = (float)cmd->dist;
    ROS_INFO("Target set at: %f", target);
    follow = cmd->follow;
    prev_kin_d_left = kin_d_left;
    prev_kin_d_right = kin_d_right;

    prev_d_left = kin_d_left;
    prev_d_right = kin_d_right;
  }
}

void KinectController::encoderCallback(const world_perception::pos_feedback::ConstPtr& data){
  prev_left_enc_pos = left_enc_pos;
  prev_right_enc_pos = right_enc_pos;
  left_enc_pos = data->left_pos;
  right_enc_pos = data->right_pos;
}

void KinectController::findGapCallback(const std_msgs::Bool::ConstPtr& m){
     printf("Initializing gap-finding \n");
    //Simple way
    ScanForGap();
    if(!gap2_found && gap1_found){
      printf("Only left edge found, mot moving.");
    }
    else if(!gap1_found){
      printf("No edge found, not moving.");
    }
    else{
      printf("Jump1: %d , Jump2: %d , Loc1: %i Loc2: %i \n", gap1_found, gap2_found, gap1_index, gap2_index);
      float gap_width = scan_data[gap1_index-1]*sin((gap2_index - gap1_index)*angle_increment); //Really bad metrics
      printf("Gap width: %f ",gap_width); 

      //Check if the gap is of the right size
      if(gap_width > base_width){
        printf(" , large enough for robot. \n");

        //-----FROM HERE: Turn to gap using only Kinect values as encoders gon' haywire.------//
        go_gap = true;
        goal_index = (gap1_index + gap2_index) / 2;
        printf("Current middle index: %i, middle index: %i \n", goal_index, i_mid);
  //go_gap = false;
      }
     else{
        printf(" , too small for robot.\n");
      }
    }
  }


void KinectController::goThroughGap(){
  if(go_gap){
        if(goal_index - i_mid > 3){
            ScanForGap();
            goal_index = (gap1_index + gap2_index) / 2;
            printf("Current middle index: %i, indexes to go: %i \n", goal_index, goal_index - i_mid);
            
            //Turn slightly right until close enough
            world_perception::command_pos posmsg;
            posmsg.theta = 0.5;
            posmsg.x = 0;
            position_pub.publish(posmsg);
            }
          else if(i_mid - goal_index > 3){
            ScanForGap();
            goal_index = (gap1_index + gap2_index) / 2;
            printf("Current middle index: %i, indexes to go: %i \n", goal_index, i_mid-goal_index);
            
            //Turn slightly right until close enough
            world_perception::command_pos posmsg;
            posmsg.theta = -0.5;
            posmsg.x = 0;
            position_pub.publish(posmsg);
            }
          else{
            printf("Goal reached, goal_index: %f",goal_index);
            go_gap = false;
        //-----FROM HERE: Move to gap using onlyKinect values ------//

                }
              }
            }


 
void KinectController::ScanForGap(){
      int k=1;
      gap1_index = 0;
      gap2_index = 0;
      gap1_found = false;
      gap2_found = false;
      //Find incontinuities in distance measurements, convert to gap position
    for(int j = 1; j < data_size-k; j=j+k){
      if(std::abs((scan_data[j-k] - scan_data[j])) > 0.2){
        if(!gap1_found){
                  gap1_found = true;
                  gap1_index = j;
                  printf("Found left incontinuity at index %i \n",j);
                 }
        else{
          gap2_found = true;
          gap2_index = j;
                  printf("Found right incontinuity at index %i \n",j);

        }
      }
    }

}

  /* Clean but not working and difficult way  
    int left_gap = -1;
    int right_gap = -1;
    float left_gap_intensity = 0;
    float right_gap_intensity = 0;
    for(int i = 0; i < data_size; i++){
      if(dd_scan_data[i] > 1 && d_scan_data[i] > 0 && left_gap <= right_gap){
        left_gap = i;
        left_gap_intensity = dd_scan_data[i];
      }
      if(dd_scan_data[i] > 1 && d_scan_data[i] < 0 && right_gap >= left_gap){
        right_gap = i;
        right_gap_intensity = dd_scan_data[i];
      }
      i++;
    }
    right_gap = right_gap - 1;

    printf("i_left: %i, i_right: %i, l_depth: %f, r_depth: %f\n", left_gap, right_gap, scan_data[left_gap], scan_data[right_gap]);

    world_perception::command_speed cmd;

    if(left_gap != -1 && right_gap != -1){
      cmd.v = (scan_data[i_mid] - 1) * 2;
      cmd.omega = (scan_data[left_gap] - scan_data[right_gap]) + (((left_gap - i_mid) + (right_gap - i_mid)) / 2);
    } else if(left_gap != -1 && right_gap == -1){
      cmd.v = 0;
      cmd.omega = (left_gap / data_size) * 2;
    } else if(left_gap != -1 && right_gap == -2){
      cmd.v = 0;
      cmd.omega = ((data_size - right_gap) / data_size) * 2;
    }else {
      cmd.v = (scan_data[i_mid] - 1) * 10;
      cmd.omega = 0;
    }
    speed_cmd.publish(cmd);
  }
}*/

void KinectController::differentiate(){
  d_scan_data = scan_data;
  dd_scan_data = scan_data;
  d_scan_data[0] = 0;
  dd_scan_data[0] = 0;
  std_msgs::Float32 msg;
  for(int i = 1; i < data_size; i++){
    d_scan_data[i] = scan_data[i] - scan_data[i - 1];
    dd_scan_data[i] = std::abs(d_scan_data[i] - d_scan_data[i - 1]);
  }
}

int main(int argc, char **argv){
  
  ros::init(argc, argv, "kinect_controller");
  KinectController kinect_controller;

  ros::spin();
  return 0;
} 
