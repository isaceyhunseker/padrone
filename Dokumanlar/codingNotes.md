# Connection Check for Publisher  

+ **loop_rate** is a variable of "ros::Rate"  
..* **Ex:** ros::Rate loop_rate(60);  
  
`while(publisher_name.getNumSubscribers() = 0)`  
`   loop_rate.sleep();`	  