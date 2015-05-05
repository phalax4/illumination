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

//req is a vector
bool classify(illumination::ArrayData::Request &req,illumination::ArrayData::Response &res){
	std::vector<unsigned char> imgVector = req.data;
	for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); it+=3) {
		
	}
	return true;
}
int main(int argc, char ** cc){
	ros::init(argc,cc,"illumination");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("array_data", classify);

}