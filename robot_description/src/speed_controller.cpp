#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>

using namespace std::chrono_literals;

class SpeedController : public rclcpp::Node
{
public:
    SpeedController()
    : Node("speed_controller"), start_time_(std::chrono::steady_clock::now())
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/rover_base_control/cmd_vel_unstamped", 10);
        timer_ = this->create_wall_timer(50ms, std::bind(&SpeedController::publish_speed, this));
    }

private:
    void publish_speed()
    {
        auto msg = geometry_msgs::msg::Twist();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time_).count();

        if (elapsed < 5) {
            msg.linear.x = static_cast<double>(elapsed);
        } else if (elapsed < 10) {
            msg.linear.x = 5.0;
        } else if (elapsed < 15) {
            msg.linear.x = 5.0 - static_cast<double>(elapsed - 10);
        } else {
            msg.linear.x = 0.0;
            RCLCPP_INFO(this->get_logger(), "Speed: %f", msg.linear.x);
            rclcpp::shutdown();
        }

        RCLCPP_INFO(this->get_logger(), "Speed: %f", msg.linear.x);
        publisher_->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::chrono::steady_clock::time_point start_time_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SpeedController>());
    rclcpp::shutdown();
    return 0;
}
