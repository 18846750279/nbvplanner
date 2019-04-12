#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "target_test");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<geometry_msgs::Pose>("target_point", 10);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    geometry_msgs::Pose msg;
    msg.position.x = 5;
    msg.position.y = 5;
    msg.position.z = 3;

    msg.orientation.w = 1;
    msg.orientation.x = 0;
    msg.orientation.y = 0;
    msg.orientation.z = 0;

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
