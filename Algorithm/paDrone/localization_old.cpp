

#include <chrono>
#include <cstdint>
//#include <mavsdk/mavsdk.h>
//#include <mavsdk/plugins/action/action.h>
//#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <thread>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/NavSatFix.h>
#include <std_msgs/Float64MultiArray.h>

#define flight_altitude 4

float altitudeOfTakeOffPoint = 0.00;
float flightAltitude;

//void globalLocationCallback(const sensor_msgs::NavSatFix::ConstPtr& msg);
void targetSetpointCallback(const sensor_msgs::NavSatFix::ConstPtr&);
void publishTargetPoint(int counterTargetPoint,float &altitude);

bool firstTarget = true;


ros::Publisher reachTarget;
ros::Publisher goNextTarget;
//ana node dan gelen target noktasi burda tutuluyor
sensor_msgs::NavSatFix target;
std_msgs::Bool reach;    //datanin ilk olarak false yapilmasi gerekebilir

//std_msgs::Float64MultiArray targetPoints;
std::vector<float> targetPoints;
int numberOfTargetPoint = 0;
int counterTargetPoint = 1;


 //global position buraya aliniyor
 //if in icinde target noktasina varip varmadigini kontrol ediyor
void globalLocationCallback(const sensor_msgs::NavSatFix::ConstPtr& msg){
    //if (altitudeOfTakeOffPoint == 0.0){
    //    altitudeOfTakeOffPoint = msg->altitude;
    //}
    ROS_INFO("globalLocCB");
    //flightAltitude = altitudeOfTakeOffPoint + flight_altitude;
    if((msg->latitude*0.99) < target.latitude < (msg->latitude*1.01) &&
            (msg->longitude*0.99) < target.longitude < (msg->longitude*1.01) &&
            (msg->altitude*0.99) < target.altitude < (msg->altitude*1.01) && counterTargetPoint < numberOfTargetPoint){
        ROS_INFO("Global Seq: [%d]", msg->header.seq);
        ROS_INFO("Global Coordinate x: [%f], y: [%f], z: [%f]", msg->latitude,msg->longitude,msg->altitude);
        reach.data = true;
        reachTarget.publish(reach);
        counterTargetPoint++;
        publishTargetPoint(counterTargetPoint,flightAltitude);
        reach.data = false;
    }
}

// !!!!!!!!!!!!!!!!!!!!!!!!
 //target noktasi geldiginde target degiskeninin icine atiyor
//void globalSetpointCallback(const sensor_msgs::NavSatFix::ConstPtr& msg){
//    ROS_INFO("Global Setpoint Seq: [%d]", msg->header.seq);
//    ROS_INFO("Global Setpoint Coordinate x: [%f], y: [%f], z: [%f]", msg->latitude,msg->longitude,msg->altitude);
//    target.latitude = msg->latitude;
//    target.longitude = msg->longitude;
//    target.altitude = msg->altitude;
//}


void targetSetpointCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
    float lat,lon,alt;
    lat = msg->latitude;
    lon = msg->longitude;
    alt = msg->altitude;
    if(firstTarget==true){
        ROS_INFO("first target");
        target.latitude = lat;
        target.longitude = lon;
        target.altitude = alt;
        firstTarget = false;
    }
    targetPoints.push_back(lat);
    targetPoints.push_back(lon);
    targetPoints.push_back(alt);
    ROS_INFO("Konum Alindi");
    numberOfTargetPoint++;
}

// // globalde olusturulan targetPoints array msg sinin tanimlamalarinin yapilmasi
//void setArray(){
//    targetPoints.layout.dim[0].label = "NumberOfTarget";
//    targetPoints.layout.dim[0].size = 15;
//    targetPoints.layout.dim[0].stride = 3*15;
//    targetPoints.layout.dim[1].label = "LatLonAlt";
//    targetPoints.layout.dim[1].size = 3;
//    targetPoints.layout.dim[1].stride = 3;
//}


// 1 --> 0-2
// 2 --> 3-5
// 3 --> 6-8
void publishTargetPoint(int counterTargetPoint,float &altitude){
    int initialBegin;
    initialBegin = (counterTargetPoint-1) * 3;
    target.latitude = targetPoints[initialBegin];
    target.longitude = targetPoints[initialBegin+1];
    target.altitude = altitude;
    goNextTarget.publish(target);
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "localization");
    ros::NodeHandle nh;
    //GPS'den gelen veri subscribe olunuyor
    ros::Subscriber sensor_msgs_NavSatFix = nh.subscribe<sensor_msgs::NavSatFix>("/mavros/global_position/global", 1000, globalLocationCallback);
//    //takeoff_n_land'den gelecek setpoint noktalarina subscribe olunuyor
//    ros::Subscriber sensor_msgs_NavSatFix_setpoint = nh.subscribe<sensor_msgs::NavSatFix>("/mavros/global_position/globalSetpoint", 1000, globalSetpointCallback);
    //takeoff_n_land'den gelen noktaya varildiginda true degeri gonderiliyor
    reachTarget = nh.advertise<std_msgs::Bool>("reachTarget", 10);
    //telemetry'den gelen setpoint noktalari burada globalde olusturulmus olan arrayin icine atilir
    ros::Subscriber getSetpoints = nh.subscribe<sensor_msgs::NavSatFix>("targets", 1000, targetSetpointCallback);
    //bir sonraki gidilcek noktayi publish ediyor
    goNextTarget = nh.advertise<sensor_msgs::NavSatFix>("nextTarget", 10);

    //setArray();

    while(ros::ok()){

    }

    ros::Rate loop_rate(10);
    ros::spin();
    return 0;
}
























