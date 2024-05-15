# My_Rover 🚀:)

En esta práctica se ha realizado la simulación de un robot en dos entornos de Gazebo diferentes, con el objetivo de estudiar el comportamiento del robot y su modelo de colisión en mundos distintos. Para ello se ha utilizado el robot realizado en la práctica 2, un robot tipo rover modificado para que pueda recojer rocas de Marte 🚀🪐 (cajas 📦).

## PARTE A

## Creación del robot 🤖

El robot es el mismo que en la práctica 2, un robot tipo rover con un brazo robótico. Por lo tanto se ha utilizado el mismo archivo urdf que en la práctica 2 aunque ha sido necesario realizar algunas modificaciones.

Lo primero que he añadido es el link base_footprint y el base_link. Una vez añadidos, he rotado el robot para cumplir con el REP-103 y REP-105.

```xml
<!-- created with Phobos 1.0.1 "Capricious Choutengan" -->
<robot name="roverA" xmlns:xacro="http://wiki.ros.org/xacro">

  <link name="base_footprint"></link>
  <joint name="base_footprint_joint_" type="fixed">
    <origin xyz="0 0 0" rpy="0 0 0" />
    <parent link="base_footprint" />
    <child link="base_link" />
  </joint>

  <link name="base_link"></link>
  <joint name="base_link_joint_" type="fixed">
    <origin xyz="0 0 0" rpy="0 0 1.57" />
    <parent link="base_link" />
    <child link="robot" />
  </joint>
```

Gracias a el codigo [sensors](robot_description/urdf/sensors.urdf.xacro) proporcionado por el profesor , he podido añadir los sensores al robot.

Además, he añadido un sensor IMU en el centro del robot y una cámara frontal en la parte delantera del robot. Por último, he añadido una cámara que va unida al eslabón final del manipulador robot.

```xml
  <!-- sensors -->
  <xacro:include filename="$(find robot_description)/urdf/sensors.urdf.xacro" />
  <xacro:arg name="robot_ns" default="" />
  <xacro:arg name="prefix" default="" />


  <xacro:sensor_camera node_name="front_camera" node_namespace="$(arg robot_ns)"
    frame_prefix="$(arg prefix)front_camera_" parent="$(arg prefix)base_link">
    <origin xyz="1.73 0.0 -0.01" rpy="0 3.141592 0" />
  </xacro:sensor_camera>

  <xacro:sensor_camera node_name="arm_camera" node_namespace="$(arg robot_ns)"
    frame_prefix="$(arg prefix)arm_camera_" parent="$(arg prefix)Barra_pinza_fixed">
    <origin xyz="0.0 0.0 -0.01" rpy="0 3.141592 0" />
  </xacro:sensor_camera>

  <xacro:sensor_imu ns="$(arg robot_ns)" prefix="$(arg prefix)imu_" parent="$(arg prefix)robot">
    <origin xyz="0.0 0.0 0.0" rpy="0 0 0" />
  </xacro:sensor_imu>
```

### video

PONER VIDEO

## PARTE B

## Modificación del robot 🤖

Para simplificar esta parte hemos tenidoq ue eliminar el brazo robótico del robot. Tambien hemos añadido los controladores de las ruedas del robot.

```xml
   <!-- Gazebo ros control plugins-->
<xacro:arg name="config_controllers"
  default="$(find robot_description)/config/rover_controller.yaml" />
<xacro:arg name="update_rate" default="100" />
<xacro:include filename="$(find robot_description)/urdf/ros2_control.urdf.xacro" />
<xacro:ros2_control />
```

## Creación de los mundos en gazebo 🌍

Los mundos fueron proporcionados por el profesor, por lo que no he tenido que crearlos. Puedes encontrar el paquete [`msr_world` ](msr_world).

## Launchers

Para empezar la simulación, se deben lanzar los siguientes launchers:

1.  [`robot state publisher`](robot_description/launch/robot_state_publisher.launch.py): Publicación de la configuración en el topic robot_description.

```bash
ros2 launch robot_description robot_state_publisher.launch.py
```

2.  [`robot gazebo`](robot_description/launch/robot_gazebo.launch.py): Spawn del robot en gazebo y en cada uno de los mundos. Se debe configurar para cada mundo.

```bash
ros2 launch robot_description robot_gazebo.launch.py
```

3.  [`robot controllers`](robot_description/launch/robot_controllers.launch.py): Lanzamiento de los controladores para mover el robot. Cuando se lance este launcher, las ruedas se ubicaran y tomarán su posición correspondiente. En la terminal donde se haya ejecutado deben visualizarse mensajes de color verde diciendo que los dos controladores están funcionando: joint_state_broadcaster y DiffController con le nombre seleccionado para tal fin.

```bash
ros2 launch robot_description robot_controllers.launch.py
```

4.  Ya está todo listo para mover el robot. Para ello, se debe lanzar el nodo [`speed_controller`](robot_description/src/speed_controller.cpp) que se encargará de mover el robot.

```bash
ros2 run robot_description speed_controller
```

## Almacenamiento de datos

Hemos usado la herramienta `rosbag` para grabar los datos de la simulación. Para ello, se ha lanzado el siguiente comando en cada uno de los mundos:

```bash
ros2 bag record -o rosbag_plot_sand /imu__imu_plugin/out /rover_base_control/odom /joint_states
```

Esto almacenaba los datos deseados como la odometría, la velocidad de las articulaciones y los datos del sensor IMU.

## Resultados 📊
