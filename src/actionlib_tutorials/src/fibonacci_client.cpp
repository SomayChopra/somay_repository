#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <actionlib_tutorials/FibonacciAction.h>
#include <iostream>


using namespace std;
using namespace actionlib_tutorials;

typedef actionlib::SimpleActionClient<FibonacciAction> Client;

void doneCb(const actionlib::SimpleClientGoalState& state,
            const FibonacciResultConstPtr& result)
{
  
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  
}

// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}

// Called every time feedback is received for the goal
void feedbackCb(const FibonacciFeedbackConstPtr& feedback)
{
  ROS_INFO("Got Feedback of length %f",feedback->feed_x);
  difference = feedback->feed_x ;
}

int main (int argc, char **argv)
{
  ros::init(argc, argv, "fibonacci");
  ROS_INFO("Action finished finally: ");
  // create the action client
  // true causes the client to spin its own thread
  //actionlib::SimpleActionClient<actionlib_tutorials::FibonacciAction> ac("fibonacci", true);
  Client ac("fibonacci", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  FibonacciGoal goal;
  goal.initial_x = 3;
  float difference = 0.0001 ;

  while(abs(difference)>=0.000001) {
  
  goal.initial_x = goal.initial_x - difference;
  
  ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(5.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished hello: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  }

  ROS_INFO("Root estimated is :  %f ", goal.initial_x);
  
  return 0;
}
