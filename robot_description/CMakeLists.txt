cmake_minimum_required(VERSION 3.5)
project(robot_description)

# Encontrar dependencias
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

# Incluir directorios
include_directories(include)

# Agregar ejecutable para el nodo de control de velocidad
add_executable(speed_controller src/speed_controller.cpp)

# Vincular bibliotecas necesarias
ament_target_dependencies(speed_controller rclcpp geometry_msgs)

# Instalar ejecutable
install(TARGETS
  speed_controller
  DESTINATION lib/${PROJECT_NAME})

install(
  DIRECTORY config launch meshes rviz urdf 
  DESTINATION share/${PROJECT_NAME}
)

ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})

if(BUILD_TESTING)
  find_package(ament_cmake_pytest REQUIRED)
endif()

ament_package()

