

#include "ros_client.h"

RosClient::RosClient(ros::NodeHandle *nh){
	sensor_msgs_NavSatFix = nh->subscribe<geographic_msgs::GeoPoseStamped>("/mavros/global_position/global", 1000, &RosClient::globalLocationCallback,this);
	getWayPoints = nh->subscribe<geographic_msgs::GeoPoseStamped>("/targets", 1000, &RosClient::getWayPointsCallback,this);
	missionComplete = nh->subscribe<std_msgs::Bool>("/missionComplete", 1000, &RosClient::missionCompleteCallback,this);
	
	reachedWaypoint = nh->advertise<std_msgs::Bool>("/reachedWaypoint", 1000);
	sendNextWaypoint = nh->advertise<geographic_msgs::GeoPoseStamped>("/nextWaypoint", 1000);
	
	//ROS_INFO("Client Olusturuldu");
}
	
	
void RosClient::getWayPointsCallback(const geographic_msgs::GeoPoseStamped::ConstPtr& msg){
	if(msg->pose.position.latitude == 999
	&& msg->pose.position.longitude == 999
	&& msg->pose.position.altitude == 999){
		this->finished_waypoint_take = true;
		ROS_INFO("Son target geldi");
	}else{
		ROS_INFO("Target Geldi  %d", msg->header.seq);
		list_global.insert(*msg);
		list_global.printWaypoint(number_of_target+1);
		number_of_target++;
	}
}
	
	
void RosClient::globalLocationCallback(const geographic_msgs::GeoPoseStamped::ConstPtr& msg){
	current_location = *msg;
	//ROS_INFO("Callback");
	//list_global.insert(25);
}

void RosClient::missionCompleteCallback(const std_msgs::Bool::ConstPtr& msg){
	if(msg->data == true){
		mission_complete_flag = true;
		ROS_INFO("mission_complete_flag true yapildi");
		
	}
}