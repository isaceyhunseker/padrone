
#include <ros/ros.h>
#include <thread>
#include <std_msgs/Bool.h>


class RosClient{
private:
	
public:
	ros::Subscriber reachedWaypoint;
	ros::Publisher missionComplete;
	RosClient(ros::NodeHandle *nh){
		reachedWaypoint = nh->subscribe<std_msgs::Bool>("/reachedWaypoint", 1000, &RosClient::reachedWaypointCallback,this);
		missionComplete = nh->advertise<std_msgs::Bool>("/missionComplete", 1000);
	}
	
	void reachedWaypointCallback(const std_msgs::Bool::ConstPtr& msg){
		if(msg->data == true){
			std_msgs::Bool message;
			message.data = true;
			missionComplete.publish(message);
			ROS_INFO("Mission send ok message %d",message.data);
		}
	}
	
};


int main(int argc, char **argv){
	ros::init(argc, argv, "Mission");
	ros::NodeHandle nh;
	RosClient rc = RosClient(&nh);
	ros::Rate rate(200);
	
	while(rc.missionComplete.getNumSubscribers() == 0)
		rate.sleep();

	while(ros::ok()){
		ros::spinOnce();
	}
	

	
}