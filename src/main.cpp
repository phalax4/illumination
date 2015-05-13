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
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <algorithm>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <fstream>
#include "illumination/ArrayData.h"
ros::Publisher pub;
ros::ServiceClient client;
illumination::ArrayData srv;
geometry_msgs::Twist turn;
double yaw;
bool inPosition = true;
int imageNumber = 0;
//calculate HSL from RGB
//Open CV?
std::vector<int> totalPredictedTargets;
void calculateHSL(const sensor_msgs::ImageConstPtr& imgRaw){
	unsigned char r, g, b, l;  
	int max, min; 
	int arrayIndex =0;
	std::vector<unsigned char> imgVector = imgRaw->data;
	unsigned char myArray[imgVector.size()/3];  //array with just the l value
	for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); it+=3) {  //every three 
		r = *it; //gets the value from the iterator
		r /=255;  
		g = *it+1; 
		g /=255;  
		b = *it+2;
		b /=255;  
		min = *std::min_element(myArray+arrayIndex, myArray+(arrayIndex+3));
		max = *std::max_element(myArray+arrayIndex, myArray+(arrayIndex+3));
		std::cout << "The smallest element is " << min << '\n';
	  	std::cout << "The largest element is "  << max << '\n'; 
	  	l = (max+min)/2;
		myArray[arrayIndex] = l;
		arrayIndex++;
	}
}



void calculateHSI(const sensor_msgs::ImageConstPtr& imgRaw){

}

/*
//directly calculate Luminance from RGB
double gam = 1.0;//2.2; //gamma correction
void calculateLuminance(const sensor_msgs::ImageConstPtr& imgRaw){
	//Y = 0.2126 R + 0.7152 G + 0.0722 B
	std::vector<unsigned char> imgVector = imgRaw->data;
	std::vector<unsigned char> lumaArray;
	ROS_WARN("The Height of the image is: %d and the Width is: %d",imgRaw->height,imgRaw->width);
	int counter = 0;

	//interate through every 3 to get Luma Y
	double luma = 0.0;
	if(inPosition){ //max gray value is 255
		inPosition = false;
		for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); it+=3) {
	    		luma +=  0.2126* (*it) * gam;
	    		luma += 0.7152* (*(it+1)) * gam;
	    		luma += 0.0722*(*(it+2)) * gam;
	    		//ROS_INFO("The luma value is: %f",luma);
	    		luma = 0.0;
	    		lumaArray.push_back(luma);
		}
	
		srv.request.data = lumaArray;
		//srv.request.turnNumber = 

		if (client.call(srv)){
       		ROS_INFO("RESPONSE is %d", (int)srv.response.updatedImgNum);
     	}else{
       		ROS_ERROR("Failed to call service");
    	}

		ROS_WARN("Picture has been Processed!");
	}

}
*/
//recieves the raw Image
//http://docs.ros.org/api/sensor_msgs/html/msg/Image.html
double gam = 1.0;
void captureImage(const sensor_msgs::ImageConstPtr& imgRaw){
	//calculateLuminance(imgRaw);
	std::vector<unsigned char> imgVector = imgRaw->data;
	std::vector<int> lumaArray;
	ROS_WARN("The Height of the image is: %d and the Width is: %d",imgRaw->height,imgRaw->width);
	int counter = 0;

	//interate through every 3 to get Luma Y
	double luma = 0.0;
	if(inPosition){ //max gray value is 255
		inPosition = false;
		for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); it+=3) {
	    		luma +=  0.2126* (*it) * gam;
	    		luma += 0.7152* (*(it+1)) * gam;
	    		luma += 0.0722*(*(it+2)) * gam;
	    		//ROS_INFO("The luma value is: %f",luma);
	    		lumaArray.push_back(luma);
	    		luma = 0.0;
		}
	
		srv.request.data = lumaArray;
		//srv.request.turnNumber = 
		srv.request.imgNum = imageNumber;
		if (client.call(srv)){
       		ROS_INFO("RESPONSE is %d", (int)srv.response.status[0]);
       		imageNumber = (int)srv.response.status[0];
       		totalPredictedTargets.push_back((int)srv.response.status[1]);
     	}else{
       		ROS_ERROR("Failed to call service");
    	}

		ROS_WARN("Picture has been Processed!");
	}


}

void getOdom(const nav_msgs::Odometry::ConstPtr &odom_msg){
	//double yaw = tf::getYaw(odom_msg.pose.orientation);
	tf::Pose pose;
  	tf::poseMsgToTF(odom_msg->pose.pose, pose);
  	yaw = tf::getYaw(pose.getRotation());
	//ROS_INFO("The yaw right now is: %f",yaw);
}

void targetTotal(int i){
		std::cout<< ' ' <<i;
}

int main(int argc, char ** cc){

	ros::init(argc,cc,"illumination_node");
	ros::NodeHandle n;
	//ros::Subscriber sub = n.subscribe("/mobile_base/commands/velocity",1000,twirl);//what topic to subscribe to
	//ros::Subscriber sub = nh.subscribe ("/camera/depth_registered/points", 1000, cloud_sub);
	//ros::Subscriber cam_sub = n.subscribe("/camera/depth_registered/",1000,captureImage);//Image topic

	ros::Subscriber cam_sub = n.subscribe("/camera/rgb/image_color",1000,captureImage);//Image topic
	ros::Subscriber odom_sub = n.subscribe("/odom",1000,getOdom);
	pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",1000);//moving the robot

	client = n.serviceClient<illumination::ArrayData>("array_data");
	//ros::service::waitForService("array_data");
	//ros::Publisher odo = n.advertise<std_msgs::Empty>("~commands/reset_odometry",1000);
	//pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1000); //topic for segbots


 
	double ros_rate = 20.0;// x times per second
	std_msgs::Empty empty;
	ros::Rate r(ros_rate);
	//int turn_number = 1;
	double startYaw = yaw;
	turn.angular.z = 0.5; //default turn speed

	//turn.linear.x = 1;
	while (ros::ok())
	{


		
		int degree = angles::to_degrees(yaw);
		ROS_INFO("%d",degree);
/*
		if(imageNumber == 6){
			ROS_WARN("STOP");
			turn.angular.z = 0.0;
			inPosition = false;
			for_each(totalPredictedTargets.begin(),totalPredictedTargets.end(),targetTotal);
			std::cout<<'\n';
		}else{
*/	
			inPosition = true;
			/*
			if(degree==0){
				inPosition == true;
			}
			if( degree ==90|| degree == -179 || degree ==45 || degree==135 || degree == -45 || degree == -135 || degree ==-90){
				ROS_INFO("Degree is: %d",degree);
				turn.angular.z = 0.0;
				inPosition = true;

			}else{
				turn.angular.z = 0.5;
			}
*/
		//}
		pub.publish(turn);



		ros::spinOnce();
		r.sleep();
	}


}
