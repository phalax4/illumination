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


ros::Publisher pub;
geometry_msgs::Twist turn;

//calculate HSL from RGB
//Open CV?
void calculateHSL(){

}

//directly calculate Luminance from RGB
void calculateLuminance(){

}

//generate histogram
//use GNU scientific library?
void calculateHistogram(){

}

//recieves the raw Image
//http://docs.ros.org/api/sensor_msgs/html/msg/Image.html
void capture(const sensor_msgs::ImageConstPtr& imgRaw){
	//std::vector<unsigned char> imgVector = imgRaw->data;    // declares a vector of unsigned chars

	cv_bridge::CvImagePtr cv_ptr;
	cv::Mat HSVImage;
	cv::Mat ThreshImage;
	// transform ROS image into OpenCV image
	cv_ptr = cv_bridge::toCvCopy(imgRaw, sensor_msgs::image_encodings::BGR8);
	//Transform the colors into HSV
	cvtColor(cv_ptr->image,HSVImage,CV_BGR2HSV);
}


int main(int argc, char ** cc){

	ros::init(argc,cc,"illumination");
	ros::NodeHandle n;
	//ros::Subscriber sub = n.subscribe("/mobile_base/commands/velocity",1000,twirl);//what topic to subscribe to

	ros::Subscriber cam_sub = n.subscribe("/kinect_camera/rgb/image_raw",1000,capture);//Image topic

	pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",1000);//moving the robot

	//pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1000); //topic for segbots


 
	double ros_rate = 20.0;// x times per second
	ros::Rate r(ros_rate);

	while (ros::ok())
	{
	
	//need to implement turn control logic aka detect how many turns have passed
	turn.linear.x = 0.0;
	turn.linear.y = 0.0;
	turn.angular.z = 0.5;
	pub.publish(turn);

		ros::spinOnce();
		r.sleep();
	}


}
