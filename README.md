# This version is modified by Zhu Hongbiao based on the ethz-asl/nbvplanner

In this version, I replaced the stereo camera with a velodyne-16. Then the function of the whole package is now to arrive the target place in an unknown environment instead to explore the whole environment. This package is also compatible with the lidar-odometry derived from lego-loam(https://github.com/18846750279/LeGO-LOAM) which is forked from RobustFieldAutonomyLab/LeGO-LOAM. But this function is in the former version of this package. If you want to use it, follow the steps below:
cd nbvplanner
```sh
git log
git reset --hard 8927cb
cd rotors
git log
git reset --hard a023a27
```
Then you can realize autonomous navigation with nbvp and lego_loam without relying groundtruth. The following is the original description of nbvplanner.


# Receding Horizon Next Best View Planning

The next best view planner is a real-time capable exploration path planner. From the current pose it expands a tree to find a next pose that gives a high exploration gain. This gain reflects the exploration of space that is not yet (sufficiently) known. As the vehicle proceeds on the path, the tree is recomputed, taking into account the new information from the sensor.

This README gives a short overview. For more information refer to the [wiki](https://github.com/ethz-asl/nbvplanner/wiki).

# Planner installation and execution

To run the current version, compile the package nbvplanner. To get it navigate to the source folder of your ros workspace:

Moreover, make sure you have all the necessary libraries:
```sh
apt-get install ros-<distro>-octomap-*
apt-get install python-catkin-tools
catkin build
```

For a simulation demo launch

```sh
roslaunch interface_nbvp_rotors flat_exploration.launch
```

Tested under ROS Indigo, Jade and Kinetic.

Further instructions for the visualization of the exploration progress, as well as more demo scenarios and descriptions of the parameters can be found in the [wiki](https://github.com/ethz-asl/nbvplanner/wiki).


If you use this software in a scientific publication, please cite the following paper:
```
@inproceedings{bircher2016receding,
  title={Receding horizon "next-best-view" planner for 3D exploration},
  author={Bircher, Andreas and Kamel, Mina and Alexis, Kostas and Oleynikova, Helen and Siegwart, Roland},
  booktitle={2016 IEEE International Conference on Robotics and Automation (ICRA)},
  pages={1462--1468},
  year={2016},
  organization={IEEE}
}
```


