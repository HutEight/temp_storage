#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <vector>
#include "std_msgs/String.h"
#include <rn_virtual_polaris/PolarisMessage.h>
using namespace std;



int main(int argc, char** argv){  
  //std::vector <std::vector <double> > data_csv;
  std::vector <double> record_csv(17,0);
  ros::init(argc, argv, "polaris_tf_listener");
  ros::NodeHandle node;

  ros::Publisher marker_origins = node.advertise<rn_virtual_polaris::PolarisMessage>("polaris/points", 1000);
  tf::TransformListener listener_1;
  tf::TransformListener listener_2;
 
  rn_virtual_polaris::PolarisMessage polaris_msg;

  ros::Rate rate(30.0);

  while (node.ok()){
    tf::StampedTransform transform_1;
    tf::StampedTransform transform_2;
    try{
      listener_1.lookupTransform("/one_tool_tip_link", "/virtual_polaris_link",  
                               ros::Time(0), transform_1);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    try{
      listener_2.lookupTransform("/two_tool_tip_link", "/virtual_polaris_link",  
                               ros::Time(0), transform_2);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

  ros::Time current_t = ros::Time::now();
  //double current_t_d = reinterpret_cast<double &>(current_t);
  polaris_msg.time =  current_t;
  polaris_msg.psm1_x = transform_1.getOrigin().x();
  polaris_msg.psm1_y = transform_1.getOrigin().y();
  polaris_msg.psm1_z = transform_1.getOrigin().z();
  polaris_msg.psm2_x = transform_2.getOrigin().x();
  polaris_msg.psm2_y = transform_2.getOrigin().y();
  polaris_msg.psm2_z = transform_2.getOrigin().z();

  marker_origins.publish(polaris_msg);
  cout<< polaris_msg.time<<"  " <<polaris_msg.psm1_x<<"  "<< polaris_msg.psm1_y<<"  "<< polaris_msg.psm1_z<<"  "<<polaris_msg.psm2_x<<"  "<< polaris_msg.psm2_y<<"  "<< polaris_msg.psm2_z<<"  "<< "\n";
  rate.sleep();
  }
  return 0;
};
