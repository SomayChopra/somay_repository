#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib_tutorials/FibonacciAction.h>
#include <iostream>


class FibonacciAction
{
protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<actionlib_tutorials::FibonacciAction> as_;
  std::string action_name_;
  // create messages that are used to publish feedback/result
  actionlib_tutorials::FibonacciFeedback feedback_;
  actionlib_tutorials::FibonacciResult result_;

public:

  FibonacciAction(std::string name) :
    as_(nh_, name, boost::bind(&FibonacciAction::executeCB, this, _1), false),
    action_name_(name)
  {
    as_.start();
  }

  ~FibonacciAction(void)
  {
  }

  void executeCB(const actionlib_tutorials::FibonacciGoalConstPtr &goal)
  {
    // helper variables
    ros::Rate r(2);
    bool success = true;

    // push_back the seeds for the fibonacci sequence
   // feedback_.feedback_x.clear();
   // feedback_.feedback_x.push_back(0);
    //

   
     float x,dash,difference,f ;

     x = goal->initial_x;
     dash = 3*x*x - 5 ;
     f = x*x*x - 5*x + 13 ;
     difference = f/dash ; 

    feedback_.feed_x = difference ;
    as_.publishFeedback(feedback_);

    r.sleep();
    if(success)
    {
     result_.result_x = feedback_.feed_x;
      ROS_INFO("%s: Succeeded", action_name_.c_str());
      // set the action state to succeeded
      as_.setSucceeded(result_);
    }
  }
};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "test_fibonacci");

  FibonacciAction fibonacci("fibonacci");
  ros::spin();

  return 0;
}
