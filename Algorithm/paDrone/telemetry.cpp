#include <chrono>
#include <cstdint>
#include <thread>
#include <iostream>
#include <ros/ros.h>
#include <inttypes.h>
#include <sensor_msgs/NavSatFix.h>
#include <mavros/mavros_plugin.h>
#include <mavros_msgs/WaypointList.h>

uint16_t waypointNumber;

mavros_msgs::WaypointList wayPoints;

void getWaypointListFromGuiCallback(const mavros_msgs::WaypointList::ConstPtr& wayPointMsg){
    //printf("getWaypointListFromGuiCallback");
    wayPoints.current_seq = wayPointMsg->current_seq;
    wayPoints.waypoints =  wayPointMsg->waypoints;
    //printf("%f",wayPoints.waypoints[0].x_lat);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "telemetry");
    ros::NodeHandle nh;
    ros::Subscriber getWaypointListFromGui = nh.subscribe<mavros_msgs::WaypointList>
            ("/mavros/mission/waypoints", 1000, getWaypointListFromGuiCallback);
    ros::spinOnce();
    ros::Rate loop_rate(10);
    ros::spin();
    return 0;
}
