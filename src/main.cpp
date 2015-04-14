#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <cmath>
#include "angles/angles.h"

#include "std_msgs/String.h"
#include "sensor_msgs/PointCloud2.h"
#include <pcl/point_types.h>
#include <pcl_ros/transforms.h>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include <cstdio>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>

void move(){

}

int main(int argc, char ** cc){

	ros::init(argc,cc,"Illumination");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/cmd_vel",1000,move);//what topic to subscribe to
	ros::Publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1000);
	ros::ServiceServer service = n.advertiseService("target_pose", nextPoint);

	//client = n.serviceClient<talker::zircon>("/coordinates");
	//service.waitForExistence();//waits for the serivce to be advertised. always use this?

 
	double ros_rate = 3.0;
	ros::Rate r(ros_rate);
	while (ros::ok())
	{
		ros::spinOnce();
		r.sleep();

	}


}