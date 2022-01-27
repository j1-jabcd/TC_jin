#!/bin/bash
source /home/jlc/Ajin_tracter_ros/src/Fast-tracker-main/devel/setup.bash
{	
sleep 2
 roslaunch -v traj_server traj_server.launch&
sleep 2
gnome-terminal --tab -t "plan_manage" -- bash -c "roslaunch -v plan_manage tracking_model.launch;exec bash"&
#roslaunch rplidar_ros rplidar.launch & sleep 2;
}
source /home/jlc/Ajin_tracter_ros/src/Fast-tracker-main/devel/setup.bash
{
sleep 3
gnome-terminal --tab -t "jlc_key" -- bash -c "rosrun car_planner jlc_node;exec bash"&
}
