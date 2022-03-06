#ifndef WAYPOINT_LIST_H
#define WAYPOINT_LIST_H


#include <ros/ros.h>
#include <thread>
#include <geographic_msgs/GeoPoseStamped.h>


class WaypointList{
private:
	struct Waypoint{
		geographic_msgs::GeoPoseStamped value;
		Waypoint *next;
	};
	
	int number_of_waypoint = 0;
    struct Waypoint *head;
    struct Waypoint *tail;
public:

	WaypointList();
	void insert(geographic_msgs::GeoPoseStamped value);
	void printWaypoint(int num);
	float * returnWaypoint (int place);
	geographic_msgs::GeoPoseStamped returnNextWaypoint(int place);
	
};


#endif /* WAYPOINT_LIST_H */