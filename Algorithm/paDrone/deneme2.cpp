
#include <ros/ros.h>
#include <thread>
#include <geographic_msgs/GeoPoseStamped.h>
#define HZ (60)

class Client{
private:
	
public:
	ros::Publisher sendWaypoint;
	Client(ros::NodeHandle *nh){
		sendWaypoint = nh->advertise<geographic_msgs::GeoPoseStamped>("/targets", 1000);
	}
};


int main(int argc, char **argv){
	ros::init(argc, argv, "LocalizationTest");
	ros::NodeHandle nh;
	Client dn = Client(&nh);	
	geographic_msgs::GeoPoseStamped empty_message;
	geographic_msgs::GeoPoseStamped last_message;
	last_message.pose.position.latitude = 999;
	last_message.pose.position.longitude = 999;
	last_message.pose.position.altitude = 999;
	empty_message.pose.position.latitude = 1.0;
	ros::Rate loop_rate(HZ);
	int k = 0;
	//This while loob must be written for every publisher
	//Bir tane subscriber baglandimi gondermeye basliyor
	//Bu topice baglanan 2 node varsa birisinin baglanmasini bekler
	//'0' degeri duruma gore degistirilmeli
	while(dn.sendWaypoint.getNumSubscribers() == 0)
		loop_rate.sleep();
	
	dn.sendWaypoint.publish(empty_message);
	dn.sendWaypoint.publish(empty_message);
	dn.sendWaypoint.publish(empty_message);
	dn.sendWaypoint.publish(last_message);
	//while(ros::ok()){
	//for(int i=0; i<100 ; i++){
	//while(k<1000){	
	//	empty_message.header.seq = k;
	//	dn.sendWaypoint.publish(empty_message);	
	//	ROS_INFO("Gonderdim %d", k);
	//	k++;
	//}
	//}
	
	
	ros::spin();
}