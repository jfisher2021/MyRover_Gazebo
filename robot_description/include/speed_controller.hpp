#ifndef SPEED_CONTROLLER_HPP
#define SPEED_CONTROLLER_HPP

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>

class SpeedController : public rclcpp::Node
{
public:
    SpeedController();

private:
    void publish_speed();
    
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::chrono::steady_clock::time_point start_time_;
};

#endif // SPEED_CONTROLLER_HPP