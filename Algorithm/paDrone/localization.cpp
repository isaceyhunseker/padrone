
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/NavSatFix.h>



class WayPoint
{
private:

    //int sequenceOfWayPoint;

public:
    float latitude;
    float longitude;
    float altitude;

    ros::Publisher WayPointPub;
    ros::Subscriber sensor_msgs_NavSatFix;//subscriber yerine client yapisina gecilebilir
    ros::Subscriber getWayPoints;
    WayPoint* previous;
    WayPoint* next;

    WayPoint();
    WayPoint(float, float, float);
    WayPoint(ros::NodeHandle *nh);
    void globalLocationCallback(const sensor_msgs::NavSatFix::ConstPtr&);
    void targetWayPointCallback(const sensor_msgs::NavSatFix::ConstPtr& msg);
    //void publishTargetPoint(int,float);
};


class WayPointList
{
public:
    int length;
    WayPoint* head;

    WayPointList();
    ~WayPointList();
//    void add();
    void addWaypoint(WayPoint &x);
};

WayPointList list;
int counter = 0;
WayPoint globalWaypoint;
sensor_msgs::NavSatFix target;


int main(int argc, char **argv){
    ros::init(argc, argv, "localization");
    ros::NodeHandle nh;
    //WayPointList list;
    WayPoint mainNode = WayPoint(&nh);


    while(mainNode.getWayPoints.getNumPublishers()==0 && mainNode.sensor_msgs_NavSatFix.getNumPublishers()==0)
    {
       ROS_ERROR("Waiting for subscibers");
       sleep(5);
    }
    ROS_ERROR("Got subscriber");

    ROS_INFO("Checkpoint1");
    ros::Duration(5).sleep();
    ROS_INFO("Checkpoint2");

    while(ros::ok()){

    }

    ros::spin();
}


/*
 *
 *                      WAYPOINTLIST
 *
 */

WayPointList::WayPointList()
{
    this->length = 0;
    this->head = NULL;
}

WayPointList::~WayPointList()
{

}

//WayPointList::add()
//{
//    WayPoint* new_node = new WayPoint();
//    WayPoint* last = this->head;

//    if(length==0){
//        this->head = new_node;
//    }else{
//         while(last->next != NULL)
//            last = last->next;
//         last->next = new_node;
//    }
//    length++;
//}

void WayPointList::addWaypoint(WayPoint &x)
{
    //WayPoint* new_node = new WayPoint();
    if(length==0){
        //new_node = x;
        this->head = &x;
        head->next = head;
        head->previous = head;
    }else{
        //new_node = x;
        head->previous->next = &x;
        x.previous = head->previous;
        x.next = head;
        head->previous = &x;
    }

    length++;
}

/*
 *
 *
 *                      WAYPOINT
 *
 */

WayPoint::WayPoint()
{

}

WayPoint::WayPoint(float lon, float lat, float alt):longitude(lon),latitude(lat),altitude(alt)
{

}

WayPoint::WayPoint(ros::NodeHandle *nh)
{
    ROS_INFO("Pub/Sub Waypointi Olusuturuluyor");
    //gps subscirebiri burada her obje icin ayri ayri mi yazilcak yoksa mainde bir kereligine mi yazilcak.
    sensor_msgs_NavSatFix = nh->subscribe<sensor_msgs::NavSatFix>("/mavros/global_position/global", 1000, &WayPoint::globalLocationCallback,this);
    //telemetry subscriberi burada mi olacak
    getWayPoints = nh->subscribe<sensor_msgs::NavSatFix>("targets", 1000, &WayPoint::targetWayPointCallback,this);
    ros::Publisher WayPointPub = nh->advertise<sensor_msgs::NavSatFix>("goPosition",1000);//action noduna latlon ve gerekli diger verileri gonderen publisher
    ROS_INFO("Pub/Sub Waypointi Olusuturuldu");
}

void WayPoint::globalLocationCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
    // GPS den data geldikce buraya giriliyor
    ROS_INFO("GPS Verisi Okundu");
    if(counter==0){
        globalWaypoint.latitude = list.head->latitude;
        globalWaypoint.longitude = list.head->longitude;
        globalWaypoint.altitude = list.head->altitude;
        globalWaypoint.next = list.head->next;
        globalWaypoint.previous = list.head->previous;

        target.latitude=globalWaypoint.latitude;
        target.longitude=globalWaypoint.longitude;
        target.altitude=globalWaypoint.altitude;
        WayPointPub.publish(target);
        counter++;
    }
    if((msg->latitude*0.99) < target.latitude < (msg->latitude*1.01) &&
            (msg->longitude*0.99) < target.longitude < (msg->longitude*1.01) &&
            (msg->altitude*0.99) < target.altitude < (msg->altitude*1.01) && counter!=0){
        globalWaypoint.latitude = globalWaypoint.next->latitude;
        globalWaypoint.longitude = globalWaypoint.next->longitude;
        globalWaypoint.altitude = globalWaypoint.next->altitude;
        globalWaypoint.next = globalWaypoint.next->next;
        globalWaypoint.previous = globalWaypoint.next->previous;

        target.latitude=globalWaypoint.latitude;
        target.longitude=globalWaypoint.longitude;
        target.altitude=globalWaypoint.altitude;
        WayPointPub.publish(target);
    }


}

void WayPoint::targetWayPointCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
    // Gelen Waypointler buraya giriyor
    //WayPoint newPoint = WayPoint(msg->latitude,msg->longitude,msg->altitude);
    //list.addWaypoint(newPoint);
    ROS_INFO("targetWayPointCallback");
}

//void WayPoint::publishTargetPoint(int counterTargetPoint,float altitude)
//{

//}



