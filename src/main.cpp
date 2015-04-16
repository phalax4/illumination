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
#include <vector>
//#include <opencv/highgui.h>
//#include <opencv/cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//#include <algorithm.h>
#include <nav_msgs/Odometry.h>
ros::Publisher pub;
geometry_msgs::Twist turn;
double yaw;
//calculate HSL from RGB
//Open CV?
void calculateHSL(const sensor_msgs::ImageConstPtr& imgRaw){

}


void calculateHSI(const sensor_msgs::ImageConstPtr& imgRaw){

}

//directly calculate Luminance from RGB
//calcalat Luma
double calculateLuminance(const sensor_msgs::ImageConstPtr& imgRaw){
	//Y = 0.2126 R + 0.7152 G + 0.0722 B
	
	std::vector<unsigned char> imgVector = imgRaw->data; 
	int count = 0;
	//interate through every 3 to get Luma Y
	double luma = 0.0;
	//for_each();
	for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); ++it) {
		if(count==0){
    		luma =  0.2126* (*it);
    	}else if(count==1){
    		luma = 0.7152* (*it);
    	}else{
    		luma = 0.0722*(*it);
    		//store luma somewhere
    		count = 0;
    	}
    	//std::cout << *it;
	}
	return luma;
}
// /camera/rgb/image_mono
// /camera/rgb/image_color
void writeToTrainingSet(){

}

void readFromTrainingSet(){

}

void Classify(){

}
//generate histogram
//use GNU scientific library?
void calculateHistogram(){

}

//recieves the raw Image
//http://docs.ros.org/api/sensor_msgs/html/msg/Image.html
void captureImage(const sensor_msgs::ImageConstPtr& imgRaw){
	//std::vector<unsigned char> imgVector = imgRaw->data;    // declares a vector of unsigned chars

	//cv_bridge::CvImagePtr cv_ptr;
	//cv::Mat HSVImage;
	//cv::Mat ThreshImage;
	// transform ROS image into OpenCV image
	//cv_ptr = cv_bridge::toCvCopy(imgRaw, sensor_msgs::image_encodings::BGR8);
	//Transform the colors into HSV
	//cvtColor(cv_ptr->image,HSVImage,CV_BGR2HSV);
}

void getOdom(const nav_msgs::Odometry::ConstPtr &odom_msg){
	//double yaw = tf::getYaw(odom_msg.pose.orientation);
	tf::Pose pose;
  	tf::poseMsgToTF(odom_msg->pose.pose, pose);
  	yaw = tf::getYaw(pose.getRotation());
	ROS_INFO("The yaw right now is: %f",yaw);
}

int main(int argc, char ** cc){

	ros::init(argc,cc,"illumination");
	ros::NodeHandle n;
	//ros::Subscriber sub = n.subscribe("/mobile_base/commands/velocity",1000,twirl);//what topic to subscribe to

	ros::Subscriber cam_sub = n.subscribe("/kinect_camera/rgb/image_raw",1000,captureImage);//Image topic
	ros::Subscriber odom_sub = n.subscribe("/odom",1000,getOdom);
	pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",1000);//moving the robot

	//pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1000); //topic for segbots


 
	double ros_rate = 20.0;// x times per second
	ros::Rate r(ros_rate);
	int turn_number = 1;	
	while (ros::ok())
	{

	if((turn_number==0) || (yaw<=-0.009 && yaw >=-0.1 )){
		turn.angular.z = 0.0;
		turn_number = turn_number-1;
	}else{
		turn.angular.z = 0.5;


	}
	//need to implement turn control logic aka detect how many turns have passed
	turn.linear.x = 0.0;
	turn.linear.y = 0.0;
	pub.publish(turn);



		ros::spinOnce();
		r.sleep();
	}


}
