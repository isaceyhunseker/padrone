/**
 * @file offb_node.cpp
 * @brief Offboard control example node, written with MAVROS version 0.19.x, PX4 Pro Flight
 * Stack and tested in Gazebo SITL
 */


/*
 *  v_0.0.5.6
 *  Bu versiyonda dron kalkar telemetry nodundan gelen komutlar arasinda dolasir.
 */

#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"
#include <map>
#include <ros/ros.h>
#include <ros/console.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Temperature.h>
#include <sensor_msgs/NavSatFix.h>
#include <geographic_msgs/GeoPoseStamped.h>
#include <string>

double flight_altitude = 0.000005;
double flightAltitude;

using namespace std;

geographic_msgs::GeoPoseStamped target;
sensor_msgs::NavSatFix initialGpsPoint;

string currentState;
mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
    currentState = current_state.mode;
    cout<<currentState<<endl;
}

void locationCallback(const geographic_msgs::GeoPoseStamped::ConstPtr& nextPointGpsMsg){
   // ROS_INFO("received_lat: [%f]", nextPointGpsMsg->latitude);
   //ROS_INFO("received_lon: [%f]", nextPointGpsMsg->longitude);
    ROS_INFO("received_alt: [%f]", nextPointGpsMsg->pose.position.altitude);

    target.pose.position.latitude = nextPointGpsMsg->pose.position.latitude;
    target.pose.position.longitude = nextPointGpsMsg->pose.position.longitude;
    target.pose.position.altitude = nextPointGpsMsg->pose.position.altitude;

    //mesajin landing kismi true ise
    //land flag 1
    }

bool move(geographic_msgs::GeoPoseStamped the_pose,ros::Publisher &global_pos_pub, ros::Rate &rate);

int main(int argc, char **argv)
{

    ros::init(argc, argv, "action");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Subscriber goNextTarget = nh.subscribe<geographic_msgs::GeoPoseStamped>
            ("nextTarget", 10,locationCallback);
    ros::Publisher global_pos_pub = nh.advertise<geographic_msgs::GeoPoseStamped>
            ("mavros/setpoint_position/global", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    while(target.pose.position.latitude == 0.0){
        cout<<"waiting the target"<<endl;
        ros::spinOnce();
        rate.sleep();
    }

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    for(int i = 10; ros::ok() && i > 0; --i){
        move(target,global_pos_pub,rate);
       }

    mavros_msgs::SetMode offb_set_mode;
    mavros_msgs::CommandBool arm_cmd;

    while(current_state.mode != "OFFBOARD" && !offb_set_mode.response.mode_sent){
        offb_set_mode.request.base_mode = 0;
        offb_set_mode.request.custom_mode = "OFFBOARD";
        set_mode_client.call(offb_set_mode);
        offb_set_mode.response.mode_sent;
        cout<<"offb_set_mode.request.custom_mode = OFFBOARD"<<endl;
     }


    while(!current_state.armed && !arm_cmd.response.success){
        arm_cmd.request.value = true;
        arming_client.call(arm_cmd);
        cout<<"arming_client.call(arm_cmd)"<<endl;
    }

    while(ros::ok()){

          move(target,global_pos_pub,rate);
          /*
           if(land flag 1){
            offb_set_mode.request.custom_mode = "AUTO.LAND";
            set_mode_client.call(offb_set_mode);
            }
           */
    }

    return 0;
}

bool move(geographic_msgs::GeoPoseStamped the_pose,ros::Publisher &global_pos_pub, ros::Rate &rate){
    global_pos_pub.publish(the_pose);
    ros::spinOnce();
    rate.sleep();
    return true;
}
