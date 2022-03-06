
#include "next_node_generation.h"

NextNodeGeneration::NextNodeGeneration(RosClient *rc_){
	this->rc = rc_;
	this->reached_wp.data = false;
	ROS_INFO("NextNodeGeneration olusturuldu %d", rc->number_of_target);
}

void NextNodeGeneration::positionControl(){
	float next_target_lat = *(this->rc->list_global.returnWaypoint(this->current_order));
	float next_target_lon = *(this->rc->list_global.returnWaypoint( this->current_order)+1);
	// If kosulunun testleri yapilmadi
	if((this->rc->current_location.pose.position.latitude * lower_error < next_target_lat
	&& this->rc->current_location.pose.position.latitude * higher_error > next_target_lat
	&& this->rc->current_location.pose.position.longitude * lower_error < next_target_lon
	&& this->rc->current_location.pose.position.longitude * higher_error  > next_target_lon
	&& reached_wp.data == false) || current_order == 0){
		this->reached_wp.data = true;
		this->rc->reachedWaypoint.publish(reached_wp);
		ROS_INFO("1.ye girdi");
		if(current_order==0){
			current_order++;
		}
	}else if(this->reached_wp.data == true && this->rc->mission_complete_flag == true){
		ROS_INFO("2.ye girdi");
		reached_wp.data = false;
		rc->mission_complete_flag = false;
		//next_node'ye gecis yapilcak
		++current_order;
		this->rc->sendNextWaypoint.publish(this->rc->list_global.returnNextWaypoint(current_order));
	}
} 