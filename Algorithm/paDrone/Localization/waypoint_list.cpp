

#include "waypoint_list.h"



WaypointList::WaypointList(){
	head = nullptr;
	tail = nullptr;
}


void WaypointList::insert(geographic_msgs::GeoPoseStamped value){
	if(head == nullptr){
		struct Waypoint *temp;
		temp = new struct Waypoint;
		temp->value = value;
		temp->next = nullptr;
		head = temp;
		tail = head;
		//ROS_INFO("Insert Yapildi");
	}else{
		struct Waypoint *temp;
		temp = new struct Waypoint;
		temp->value = value;
		temp->next = nullptr;
		if(head->next == nullptr){
			head->next = temp;
		}else{
			tail->next = temp;
		}
		tail = temp;
	}
	number_of_waypoint++;
	//ROS_INFO("subscribedan geldi");
}
	
	
void WaypointList::printWaypoint(int num){
	struct Waypoint *current = head;
	for(int i=1; i<num ; i++){
		current = current->next;
	} 
	ROS_INFO("%d  ",current->value.header.seq);
}


float * WaypointList::returnWaypoint( int place){
	//arr[0] latitude degerini arr[1] ise longitude degerini donmektedir
	static float arr[2] = {0,0};
	if(number_of_waypoint == 0){
		return arr;
	}
	
	struct Waypoint *iter = head;
	for(int i=1; i<place ; i++){
		iter = iter->next;
	} 
	arr[0] = iter->value.pose.position.latitude;
	arr[1] = iter->value.pose.position.longitude;
	return arr;
}


geographic_msgs::GeoPoseStamped WaypointList::returnNextWaypoint(int place){
	struct Waypoint *iter = head;
	for(int i=1; i<place ; i++){
		iter = iter->next;
	} 
	return iter->value;
}



















