// Copyright (c) 2009, Willow Garage, Inc.
// Copyright (c) 2017, Open Source Robotics Foundation, Inc.
// All rights reserved.
//
// Software License Agreement (BSD License 2.0)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//  * Neither the name of the Willow Garage nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


//\author: Blaise Gassend

// #include <diagnostic_updater/diagnostic_updater.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <math.h>
#include <unistd.h>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>

using namespace std::chrono_literals;
// TODO(mikaelarguedas) remove these macros to use rclcpp logging macro when available
// add back the pedantic flag at that point because
// we won't have variadic macros with empty VA_ARGS anymore
#define ROS_ERROR(str, ...) fprintf(stderr, str "\n", ## __VA_ARGS__)
#define ROS_WARN(str, ...) fprintf(stderr, str "\n", ## __VA_ARGS__)
#define ROS_INFO(str, ...) printf(str "\n", ## __VA_ARGS__)
#define ROS_DEBUG(str, ...) printf(str "\n", ## __VA_ARGS__)

///\brief Opens, reads from and publishes joystick events
class Joystick
{
private:
  // bool open_;               
  std::string joy_dev_;
  double deadzone_;
  double autorepeat_rate_;  // in Hz.  0 for no repeat.
  double coalesce_interval_; // Defaults to 100 Hz rate limit.

  // // TODO(mikaelarguedas) commenting out diagnostic logic for now
  // int event_count_;
  // int pub_count_;
  // double lastDiagTime_;
  // 
  // diagnostic_updater::Updater diagnostic_;
  // 
  // ///\brief Publishes diagnostics and status
  // void diagnostics(diagnostic_updater::DiagnosticStatusWrapper& stat)
  // {
  //   double now = rclcpp::Time::now().toSec();
  //   double interval = now - lastDiagTime_;
  //   if (open_)
  //     stat.summary(0, "OK");
  //   else
  //     stat.summary(2, "Joystick not open.");
  //   
  //   stat.add("topic", pub_.getTopic());
  //   stat.add("device", joy_dev_);
  //   stat.add("dead zone", deadzone_);
  //   stat.add("autorepeat rate (Hz)", autorepeat_rate_);
  //   stat.add("coalesce interval (s)", coalesce_interval_);
  //   stat.add("recent joystick event rate (Hz)", event_count_ / interval);
  //   stat.add("recent publication rate (Hz)", pub_count_ / interval);
  //   stat.add("subscribers", pub_.getNumSubscribers());
  //   event_count_ = 0;
  //   pub_count_ = 0;
  //   lastDiagTime_ = now;
  // }
  //

public:
  Joystick()
  {}
  
  ///\brief Opens joystick port, reads from port and publishes while node is active
  int main(int argc, char **argv)
  {
    // diagnostic_.add("Joystick Driver Status", this, &Joystick::diagnostics);
    // diagnostic_.setHardwareID("none");
    (void)argc;
    (void)argv;

    auto node = std::make_shared<rclcpp::Node>("joy_node");

    auto pub = node->create_publisher<sensor_msgs::msg::Joy>("joy");

    // Parameters
    node->get_parameter_or("dev", joy_dev_, std::string("/dev/input/js0"));
    node->get_parameter_or("deadzone", deadzone_, 0.05);
    node->get_parameter_or("autorepeat_rate", autorepeat_rate_, static_cast<double>(20));
    node->get_parameter_or("coalesce_interval", coalesce_interval_, 0.001);
    

    // Checks on parameters
    if (autorepeat_rate_ > 1 / coalesce_interval_)
      ROS_WARN("joy_node: autorepeat_rate (%f Hz) > 1/coalesce_interval (%f Hz) does not make sense. Timing behavior is not well defined.", autorepeat_rate_, 1/coalesce_interval_);
    
    if (deadzone_ >= 1)
    {
      ROS_WARN("joy_node: deadzone greater than 1 was requested. The semantics of deadzone have changed. It is now related to the range [-1:1] instead of [-32767:32767]. For now I am dividing your deadzone by 32767, but this behavior is deprecated so you need to update your launch file.");
      deadzone_ /= 32767;
    }
    
    if (deadzone_ > 0.9)
    {
      ROS_WARN("joy_node: deadzone (%f) greater than 0.9, setting it to 0.9", deadzone_);
      deadzone_ = 0.9;
    }
    
    if (deadzone_ < 0)
    {
      ROS_WARN("joy_node: deadzone_ (%f) less than 0, setting to 0.", deadzone_);
      deadzone_ = 0;
    }

    if (autorepeat_rate_ < 0)
    {
      ROS_WARN("joy_node: autorepeat_rate (%f) less than 0, setting to 0.", autorepeat_rate_);
      autorepeat_rate_ = 0;
    }
    
    if (coalesce_interval_ < 0)
    {
      ROS_WARN("joy_node: coalesce_interval (%f) less than 0, setting to 0.", coalesce_interval_);
      coalesce_interval_ = 0;
    }
    
    // Parameter conversions
    double autorepeat_interval = 1 / autorepeat_rate_;
    double scale = -1. / (1. - deadzone_) / 32767.;
    double unscaled_deadzone = 32767. * deadzone_;

    js_event event;
    struct timeval tv;
    fd_set set;
    int joy_fd;
    // event_count_ = 0;
    // pub_count_ = 0;
    // lastDiagTime_ = rclcpp::Time::now().toSec();
    
    // Big while loop opens, publishes
    while (rclcpp::ok())
    {                                      
      // open_ = false;
      // diagnostic_.force_update();
      bool first_fault = true;
      auto timer_callback = []() -> void {};
      auto timer = node->create_wall_timer(1s, timer_callback);
      while (true)
      {
        rclcpp::spin_some(node);
        joy_fd = open(joy_dev_.c_str(), O_RDONLY);
        if (joy_fd != -1)
        {
          // There seems to be a bug in the driver or something where the
          // initial events that are to define the initial state of the
          // joystick are not the values of the joystick when it was opened
          // but rather the values of the joystick when it was last closed.
          // Opening then closing and opening again is a hack to get more
          // accurate initial state data.
          close(joy_fd);
          joy_fd = open(joy_dev_.c_str(), O_RDONLY);
        }
        if (joy_fd != -1)
          break;
        if (first_fault)
        {
          ROS_ERROR("Couldn't open joystick %s. Will retry every second.", joy_dev_.c_str());
          first_fault = false;
        }
        // diagnostic_.update();
      }
      timer->cancel();
      ROS_INFO("Opened joystick: %s. deadzone_: %f.", joy_dev_.c_str(), deadzone_);
      // open_ = true;
      // diagnostic_.force_update();
      
      bool tv_set = false;
      bool publication_pending = false;
      tv.tv_sec = 1;
      tv.tv_usec = 0;

      // Here because we want to reset it on device close.
      auto joy_msg = std::make_shared<sensor_msgs::msg::Joy>();
      joy_msg->header.stamp.sec = 0;
      joy_msg->header.stamp.nanosec = 0;
      joy_msg->header.frame_id = "joy";
      joy_msg->axes.resize(2);
      joy_msg->axes[0] = joy_msg->axes[1] = 0;
      joy_msg->buttons.resize(1);
      joy_msg->buttons[0] = 0;

      while (rclcpp::ok()) 
      {
        rclcpp::spin_some(node);
        
        bool publish_now = false;
        bool publish_soon = false;
        FD_ZERO(&set);
        FD_SET(joy_fd, &set);
        
        //ROS_INFO("Select...");
        int select_out = select(joy_fd+1, &set, NULL, NULL, &tv);
        //ROS_INFO("Tick...");
        if (select_out == -1)
        {
          tv.tv_sec = 0;
          tv.tv_usec = 0;
          continue;
          //				break; // Joystick is probably closed. Not sure if this case is useful.
        }
        
        if (FD_ISSET(joy_fd, &set))
        {
          if (read(joy_fd, &event, sizeof(js_event)) == -1 && errno != EAGAIN)
            break; // Joystick is probably closed. Definitely occurs.
          
          //ROS_INFO("Read data...");
          joy_msg->header.stamp = rclcpp::Time::now();
          // event_count_++;
          switch(event.type)
          {
          case JS_EVENT_BUTTON:
          case JS_EVENT_BUTTON | JS_EVENT_INIT:
            if(event.number >= joy_msg->buttons.size())
            {
              int old_size = joy_msg->buttons.size();
              joy_msg->buttons.resize(event.number+1);
              for(unsigned int i=old_size;i<joy_msg->buttons.size();i++)
                joy_msg->buttons[i] = 0.0;
            }
            joy_msg->buttons[event.number] = (event.value ? 1 : 0);
            // For initial events, wait a bit before sending to try to catch
            // all the initial events.
            if (!(event.type & JS_EVENT_INIT))
              publish_now = true;
            else
              publish_soon = true;
            break;
          case JS_EVENT_AXIS:
          case JS_EVENT_AXIS | JS_EVENT_INIT:
            if(event.number >= joy_msg->axes.size())
            {
              int old_size = joy_msg->axes.size();
              joy_msg->axes.resize(event.number+1);
              for(unsigned int i=old_size;i<joy_msg->axes.size();i++)
                joy_msg->axes[i] = 0.0;
            }
            if (!(event.type & JS_EVENT_INIT)) // Init event.value is wrong.
            {
              double val = event.value;
              // Allows deadzone to be "smooth"
              if (val > unscaled_deadzone)
                val -= unscaled_deadzone;
              else if (val < -unscaled_deadzone)
                val += unscaled_deadzone;
              else
                val = 0;
              joy_msg->axes[event.number] = val * scale;
            }
            // Will wait a bit before sending to try to combine events. 				
            publish_soon = true;
            break;
          default:
            ROS_WARN("joy_node: Unknown event type. Please file a ticket. time=%u, value=%d, type=%Xh, number=%d", event.time, event.value, event.type, event.number);
            break;
          }
        }
        else if (tv_set) // Assume that the timer has expired.
          publish_now = true;
        
        if (publish_now)
        {
          // Assume that all the JS_EVENT_INIT messages have arrived already.
          // This should be the case as the kernel sends them along as soon as
          // the device opens.
          //ROS_INFO("Publish...");
          pub->publish(joy_msg);
          publish_now = false;
          tv_set = false;
          publication_pending = false;
          publish_soon = false;
          // pub_count_++;
        }
        
        // If an axis event occurred, start a timer to combine with other
        // events.
        if (!publication_pending && publish_soon)
        {
          tv.tv_sec = trunc(coalesce_interval_);
          tv.tv_usec = (coalesce_interval_ - tv.tv_sec) * 1e6;
          publication_pending = true;
          tv_set = true;
          //ROS_INFO("Pub pending...");
        }
        
        // If nothing is going on, start a timer to do autorepeat.
        if (!tv_set && autorepeat_rate_ > 0)
        {
          tv.tv_sec = trunc(autorepeat_interval);
          tv.tv_usec = (autorepeat_interval - tv.tv_sec) * 1e6; 
          tv_set = true;
          //ROS_INFO("Autorepeat pending... %i %i", tv.tv_sec, tv.tv_usec);
        }
        
        if (!tv_set)
        {
          tv.tv_sec = 1;
          tv.tv_usec = 0;
        }
	
        // diagnostic_.update();
      } // End of joystick open loop.
      
      close(joy_fd);
      rclcpp::spin_some(node);
      if (rclcpp::ok())
        ROS_ERROR("Connection to joystick device lost unexpectedly. Will reopen.");
    }
    
    return 0;
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  Joystick j;
  return j.main(argc, argv);
}
