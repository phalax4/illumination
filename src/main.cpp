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
//calculate HSL from RGB
//Open CV?

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

//directly calculate Luminance from RGB
//calcalat Luma

double gam = 1.0;//2.2;
void calculateLuminance(const sensor_msgs::ImageConstPtr& imgRaw){
	//Y = 0.2126 R + 0.7152 G + 0.0722 B
	std::vector<unsigned char> imgVector = imgRaw->data;
	//int size = (imgVector.size())/3;
	//unsigned char lumaArray[size];
	std::vector<unsigned char> lumaArray;

	int counter = 0;
	//double[imgVector/3.0] lumaArray;
	//std::string 
	//ofstream myfile;
  	//myfile.open ("example.txt");
  //myfile << "Writing this to a file.\n";
  //myfile.close();
	//int count = 0;
	//interate through every 3 to get Luma Y
	double luma = 0.0;
	if(inPosition){ //max gray value is 255
		inPosition = false;
		for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); it+=3) {
			//if(count==0){
	    		luma +=  0.2126* (*it) * gam;
	    	//	count++;
	    	//}else if(count==1){
	    		luma += 0.7152* (*(it+1)) * gam;
	    	//	count++;
	    	//}else{
	    		luma += 0.0722*(*(it+2)) * gam;
	    		//count = 0;
	    		//ROS_INFO("The luma value is: %f",luma);
	    	//}
	    		//lumaArray[counter] = luma;
	    		luma = 0.0;
	    		lumaArray.push_back(luma);
	    		//counter++;
	    	//std::cout << *it;
		}
		srv.request.data = lumaArray;
		if (client.call(srv)){
       		ROS_INFO("RESPONSE is %d", (int)srv.response.success);
     	}else{
       		ROS_ERROR("Failed to call service");
       		//return 1;
       		//return 0;
    	}
		ROS_WARN("Picture has been Processed!");
	}
}

// /camera/rgb/image_mono
// /camera/rgb/image_color

//recieves the raw Image
//http://docs.ros.org/api/sensor_msgs/html/msg/Image.html
void captureImage(const sensor_msgs::ImageConstPtr& imgRaw){
	calculateLuminance(imgRaw);

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
	//ros::Subscriber sub = nh.subscribe ("/camera/depth_registered/points", 1000, cloud_sub);
	//ros::Subscriber cam_sub = n.subscribe("/camera/depth_registered/",1000,captureImage);//Image topic

	ros::Subscriber cam_sub = n.subscribe("/camera/rgb/image_color",1000,captureImage);//Image topic
	ros::Subscriber odom_sub = n.subscribe("/odom",1000,getOdom);
	pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",1000);//moving the robot

	client = n.serviceClient<illumination::ArrayData>("array_data");

	//ros::Publisher odo = n.advertise<std_msgs::Empty>("~commands/reset_odometry",1000);
	//pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1000); //topic for segbots


 
	double ros_rate = 20.0;// x times per second
	std_msgs::Empty empty;
	ros::Rate r(ros_rate);
	//int turn_number = 1;
	double startYaw = yaw;
	double startDegrees = angles::normalize_angle_positive(angles::to_degrees(startYaw));
	while (ros::ok())
	{


	//if((turn_number==0) || (yaw<=-0.009 && yaw >=-0.1 )){
	//	turn.angular.z = 0.0;
		//turn_number--; //turn_number-1;
		//odo.publish(empty); //reset the odometer as above control logic cannot guarantee precise full turn
	//}else{
		int degree = angles::to_degrees(yaw);
		ROS_INFO("%d",degree);
		if( degree ==90|| degree == 0 || degree == -179 || degree ==45 || degree==135 || degree == -45 || degree == -135){
			turn.angular.z = 0.0;
			inPosition = true;

		}else{
			turn.angular.z = 0.5;
		}


	//}
	//need to implement turn control logic aka detect how many turns have passed
	//turn.linear.x = 0.0;
	//turn.linear.y = 0.0;
		pub.publish(turn);



		ros::spinOnce();
		r.sleep();
	}


}
