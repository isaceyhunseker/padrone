
#include "next_node_generation.h"
#include "waypoint_list.h"
#include "ros_client.h"
#include <ros/ros.h>
#include <thread>
#include <sensor_msgs/NavSatFix.h>



int main(int argc, char **argv){
	ros::init(argc, argv, "Localization");
	ros::NodeHandle nh;
	RosClient rc = RosClient(&nh);	
	ros::Rate rate(200);
	while(!rc.finished_waypoint_take){
		rate.sleep();
		ros::spinOnce();
	}
	
	while(rc.reachedWaypoint.getNumSubscribers() == 0){
		rate.sleep();
		ros::spinOnce();
	}
	
	while(rc.sendNextWaypoint.getNumSubscribers() == 0){
		ROS_INFO("Simdide burada takildim");
		rate.sleep();
		ros::spinOnce();
	}
	
	
	
	/*&& rc.reachedWaypoint.getNumSubscribers() == 0
	&& rc.sendNextWaypoint.getNumSubscribers() == 0*/
	
	NextNodeGeneration nng = NextNodeGeneration(&rc);
	
	while(ros::ok()){
		nng.positionControl();
		//ROS_INFO("while donuyor");
		ros::spinOnce();
	}	
}