/*
 * Copyright 2015 Fadri Furrer, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Mina Kamel, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Janosch Nikolic, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Markus Achtelik, ASL, ETH Zurich, Switzerland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <thread>
#include <chrono>
#include <fstream>

#include <ros/ros.h>
#include <ros/package.h>
#include <tf/tf.h>
#include <std_srvs/Empty.h>
#include <mav_msgs/CommandTrajectoryPositionYaw.h>
#include <nbvplanner/nbvp_srv.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "exploration");
  ros::NodeHandle nh;
  ros::Publisher trajectory_pub = nh.advertise<mav_msgs::CommandTrajectoryPositionYaw>(
      "command/trajectory_position_yaw", 10);
  ROS_INFO("Started exploration");

  std_srvs::Empty srv;
  bool unpaused = ros::service::call("/gazebo/unpause_physics", srv);
  unsigned int i = 0;

  // Trying to unpause Gazebo for 10 seconds.
  while (i <= 10 && !unpaused) {
    ROS_INFO("Wait for 1 second before trying to unpause Gazebo again.");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    unpaused = ros::service::call("/gazebo/unpause_physics", srv);
    ++i;
  }

  if (!unpaused) {
    ROS_FATAL("Could not wake up Gazebo.");
    return -1;
  } else {
    ROS_INFO("Unpaused the Gazebo simulation.");
  }

  double dt = 1.0;
  std::string ns = ros::this_node::getName();
  if (!ros::param::get(ns + "/nbvp/dt", dt)) {
    ROS_FATAL("Could not start exploration. Parameter missing! Looking for %s",
              (ns + "/nbvp/dt").c_str());
    return -1;
  }

  // Wait for 5 seconds to let the Gazebo GUI show up.
  ros::Duration(5.0).sleep();

  mav_msgs::CommandTrajectoryPositionYaw trajectory_msg;

  ROS_INFO("Starting the planner");
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 0.2);
  trajectory_msg.yaw = 0.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);
  ros::Duration(2.0).sleep();
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 1.5);
  trajectory_msg.yaw = 1.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);
  ros::Duration(2.0).sleep();
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 3.0);
  trajectory_msg.yaw = 2.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);
  ros::Duration(2.0).sleep();
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 3.0);
  trajectory_msg.yaw = 1.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);
  ros::Duration(2.0).sleep();
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 3.0);
  trajectory_msg.yaw = 2.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);
  ros::Duration(2.0).sleep();
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 3.0);
  trajectory_msg.yaw = 3.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);
  ros::Duration(2.0).sleep();
  nh.param<double>("wp_x", trajectory_msg.position.x, 0.0);
  nh.param<double>("wp_y", trajectory_msg.position.y, 0.0);
  nh.param<double>("wp_z", trajectory_msg.position.z, 3.0);
  trajectory_msg.yaw = 3.0;
  trajectory_msg.header.stamp = ros::Time::now();
  trajectory_pub.publish(trajectory_msg);

  ros::Duration(5.0).sleep();

  std::string pkgPath = ros::package::getPath("nbvplanner");
  std::fstream file;
  file.open((pkgPath + "/data/path.m").c_str(), std::ios::out);
  if (!file.is_open()) {
    ROS_WARN("could not open path file");
  }
  file << "pathMatrix = [";
  int iteration = 0;
  std::vector<double> execution;
  std::vector<double> computation;
  while (ros::ok()) {
    ROS_INFO_THROTTLE(1, "Initiating replanning");
    nbvplanner::nbvp_srv planSrv;
    planSrv.request.header.stamp = ros::Time::now();
    planSrv.request.header.seq = iteration;
    planSrv.request.header.frame_id = ros::this_node::getNamespace();
    ros::Time start = ros::Time::now();
    if (ros::service::call("nbvplanner", planSrv)) {
      computation.push_back((ros::Time::now() - start).toSec());
      start = ros::Time::now();
      for (int i = 0; i < 100 && i < planSrv.response.path.size(); i++) {
        tf::Pose pose;
        tf::poseMsgToTF(planSrv.response.path[i], pose);
        double yaw = tf::getYaw(pose.getRotation());
        trajectory_msg.position.x = planSrv.response.path[i].position.x;
        trajectory_msg.position.y = planSrv.response.path[i].position.y;
        // adding offset 0.25 to account for tracking error of lee_position_controller
        trajectory_msg.position.z = planSrv.response.path[i].position.z+0.25;
        trajectory_msg.yaw = yaw;

        trajectory_msg.header.stamp = ros::Time::now();
        trajectory_pub.publish(trajectory_msg);
        file << planSrv.response.path[i].position.x << ", " << planSrv.response.path[i].position.y
             << ", " << planSrv.response.path[i].position.z << ", " << yaw << ", "
             << trajectory_msg.header.stamp.toSec() << ";\n";
        ros::Duration(dt).sleep();
      }
      execution.push_back((ros::Time::now() - start).toSec());
    } else {
      ROS_WARN_THROTTLE(1, "Planner not reachable");
      ros::Duration(1.0).sleep();
    }
    iteration++;
  }
  file << "];\nexecutionTime = [";
  for (int i = 0; i < execution.size(); i++) {
    file << execution[i] << ", ";
  }
  file << "];\ncomputationTime = [";
  for (int i = 0; i < computation.size(); i++) {
    file << computation[i] << ", ";
  }
  file << "];";
  file.close();
}
