#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <sstream>
#include <std_msgs/Int32.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
int32_t voicenumber;

void VoiceCallBack(const std_msgs::Int32 &msg)
{
    voicenumber = msg.data;
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"simple_navigation_goals_publisher");
    ros::NodeHandle n;
    ros::Publisher goal_pub=n.advertise<std_msgs::String>("simple_navigation_goals_publisher/robot_state",1);
    ros::Subscriber voice_sub=n.subscribe<std_msgs::Int32>("/voice_control",10,VoiceCallBack);
    MoveBaseClient ac("move_base",true);

    while(!ac.waitForServer(ros::Duration(10.0)))
    {
        ROS_INFO("Waiting for the move_base action server to come up");
    }

while(ros::ok())
{
    std_msgs::String msg;
    std::stringstream ss;

    for(int i=0;i<4;i++)
    {
        int count = i;
        move_base_msgs::MoveBaseGoal goal;
//        float goals_x[4] = {-0.2339,12.4538,13.5590,0};
//        float goals_y[4] = {9.09035,30.8866,14.9178,0};

        float goals_x[4] = {13.5590,12.4538,-0.2339,0};
        float goals_y[4] = {14.9178,30.8866,9.09035,0};

//        float goals_x[4] = {-0.2,-0.4,-0.8,0};
//        float goals_y[4] = {-0.2,-0.4,-0.8,0};

        goal.target_pose.header.frame_id = "/map";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = goals_x[i];
        goal.target_pose.pose.position.y = goals_y[i];
        goal.target_pose.pose.orientation.w = 1.0;
        ROS_INFO("Sending goal...");
        ros::Duration(5).sleep();
        ac.sendGoal(goal);
        ac.waitForResult();

//        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
//        {
//            switch (i)
//            {
//             case 0:
//                ROS_INFO("I arrived at water dispenser!");
//                ss << "I arrived at water dispenser!  Going to Boss's office...";
//                break;

//             case 1:
//                ROS_INFO("I arrived at Boss's office!");
//                ss << "I arrived at Boss's office!  Going to Boss's office...";

//                break;
//             case 2:
//                ROS_INFO("GQY gate !");
//                ss << "GQY gate ! Going to origin point...";

//                break;
//            case 3:
//                ROS_INFO("I arrived at origin point!");
//                ss << "I arrived at origin point! Going to water dispenser...";

//                break;
//             default:
//                ROS_INFO("Something is error,please check....");
//                ss << "Something is error,please check....";
//            }
//        }
        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        {
            switch (i)
            {
             case 0:
                ROS_INFO("GQY gate !");
                ss << "I arrived GQY gate!  Going to Boss's office...";
//                ss = "I arrived GQY gate!  Going to Boss's office...";
                break;

             case 1:
                ROS_INFO("I arrived at Boss's office!");
                ss << "I arrived at Boss's office!  Going to water dispenser...";

                break;
             case 2:
                ROS_INFO("I arrived at water dispenser!");
                ss << "I arrived at water dispenser! Going to origin point...";

                break;
            case 3:
                ROS_INFO("I arrived at origin point!");
                ss << "I arrived at origin point! Going to GQY gate...";

                break;
             default:
                ROS_INFO("Something is error,please check....");
                ss << "Something is error,please check....";
            }
        }
        else
        {
            ROS_ERROR("The base failed to move goal !");
            ROS_INFO("Trying anaing...,go to goal!");
            ss << "The base failed to move goal !Trying anaing...,go to goal!";
            ros::Duration(5.0).sleep();
            goal.target_pose.pose.position.x = goals_x[count];
            goal.target_pose.pose.position.y = goals_y[count];
            goal.target_pose.pose.orientation.w = 1.0;
            ac.sendGoal(goal);
            ac.waitForResult();
        }

        if(i==4)
        {
            i=0;
        }


        ros::Duration(10).sleep();
        msg.data = ss.str();
        goal_pub.publish(msg);
        ros::spinOnce();

    }
}



    return 0;
}
