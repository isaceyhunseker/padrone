
#ifndef ROS_CLIENT_H
#define ROS_CLIENT_H

#include "waypoint_list.h"
#include <ros/ros.h>
#include <thread>
#include <geographic_msgs/GeoPoseStamped.h>
#include <std_msgs/Bool.h>

class RosClient{
private:
    
	ros::Subscriber sensor_msgs_NavSatFix;
	ros::Subscriber getWayPoints;
	ros::Subscriber missionComplete;
public:
	ros::Publisher reachedWaypoint;
	ros::Publisher sendNextWaypoint;

	WaypointList list_global;
	
	bool finished_waypoint_take = false; //telemetryden gelen waypointler bittiginde kalkar
	bool mission_complete_flag = false; 
	geographic_msgs::GeoPoseStamped current_location;
	int number_of_target = 0;
	
	RosClient(ros::NodeHandle *nh);
	
	void getWayPointsCallback(const geographic_msgs::GeoPoseStamped::ConstPtr& msg);
	void globalLocationCallback(const geographic_msgs::GeoPoseStamped::ConstPtr& msg);
	void missionCompleteCallback(const std_msgs::Bool::ConstPtr& msg);
};

#endif /* ROS_CLIENT_H */