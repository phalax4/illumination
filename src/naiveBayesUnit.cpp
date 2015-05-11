#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <cmath>
#include "std_msgs/String.h"
#include <cstdio>
#include <sensor_msgs/image_encodings.h>
#include <algorithm>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <fstream>
#include "illumination/ArrayData.h"
#include <map>
#include <iterator>
#include <vector>

//req is a vector
bool doClassify(illumination::ArrayData::Request &req,illumination::ArrayData::Response &res){
	std::vector<unsigned char> imgVector = req.data;
    ROS_WARN("[RESPONSE]: DATA RECEIVED: %ld",imgVector.size());
    unsigned int imageNumber = req.imgNum;

    unsigned int bin[256];
   	for(std::vector<unsigned char>::iterator it = imgVector.begin(); it != imgVector.end(); it++) {
   		bin[*it]++;
   	}

   	
   	std::vector<int> response;
   	response.push_back(imageNumber+1);
   	response.push_back(output);
	/*
	//if light is broken return 1
	imgVector.push_back(1);
	//if light is not broken return 0
	//imgVector.push_back(2);

	std::ofstream output_file( "data.txt", std::ios_base::app ) ;  
    std::ostream_iterator<unsigned char> output_iterator(output_file, ",");
    std::copy(imgVector.begin(), imgVector.end(), output_iterator);
    output_file<<'\n';
	output_file.close();
	ROS_WARN("RESPONSE: DATA WRITTEN");
	*/
	return true;
}
	//picture #, mean, 1(True, broken) or 0(False, not broken)




int main(int argc, char ** cc){
	ros::init(argc,cc,"illumination_NaiveBayes");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("array_data", doClassify);
	ros::spin();
}
