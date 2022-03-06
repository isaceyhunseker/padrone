#ifndef NEXT_NODE_GENERATION_H
#define NEXT_NODE_GENERATION_H

#include "waypoint_list.h"
#include "ros_client.h"
#include <ros/ros.h>
#include <thread>
#include <geographic_msgs/GeoPoseStamped.h>
#include <std_msgs/Bool.h>

class NextNodeGeneration{
private:
	RosClient *rc;
	int current_order = 0;
	const float lower_error = 0.95;
	const float higher_error = 1.05;
	std_msgs::Bool reached_wp;
public:
	NextNodeGeneration(RosClient *rc);
	void positionControl();
};




#endif /* NEXT_NODE_GENERATION_H */