# My_Rover 🚀

En esta práctica se ha realizado la simulación de un robot en dos entornos de Gazebo diferentes, con el objetivo de estudiar el comportamiento del robot y su modelo de colisión en mundos distintos. Para ello se ha utilizado el robot realizado en la práctica 2, un robot tipo rover modificado para que pueda recoger rocas de Marte 🚀🪐 (cajas 📦).

## Índice 📋
1. [PARTE A](#parte-a)
    - [Creación del robot 🤖](#creación-del-robot-🤖)
2. [PARTE B](#parte-b)
    - [Modificación del robot 🤖](#modificación-del-robot-🤖)
    - [Creación de los mundos en Gazebo🌍](#creación-de-los-mundos-en-gazebo-🌍)
    - [Launchers](#launchers)
    - [Almacenamiento de datos](#almacenamiento-de-datos)
    - [Resultados📊](#resultados-📊)
    - [Comparativa de las velocidades](#comparativa-de-las-velocidades)
        - [Análisis del mundo de la arena🏜️](#análisis-del-mundo-de-la-arena-🏜️)
        - [Análisis del mundo del suelo🏞️](#análisis-del-mundo-del-suelo-🏞️)
        - [Conclusiones de la comparativa de velocidades](#conclusiones-de-la-comparativa-de-velocidades)
    - [Comparativa de la odometría y la velocidad](#comparativa-de-la-odometría-y-la-velocidad)
    - [Comparativa de las aceleraciones](#comparativa-de-las-aceleraciones)
        - [Conclusiones de la comparativa de las aceleraciones](#conclusiones-de-la-comparativa-de-las-aceleraciones)
3. [Conclusiones finales📝](#conclusiones-finales-📝)
4. [Datos adjuntos📚](#datos-adjuntos-📚)


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

[PONER VIDEO](URL_DEL_VIDEO)
PONER VIDEO

### TF2 FRAMES

[tf2_frames.pdf](data/tf2_frames.pdf)


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

Video floor

[floor_rover.webm](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/f11166cc-df0b-4b2a-8a60-18f306024a32)

video sand

[sand_rover.webm](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/362d9f3a-d2d0-4682-874d-4696bad70ce8)

## Comparativa de las velocidades


### Análisis del mundo de la arena 🏜️

Se puede apreciar que el robot se mueve con un poco mas de dificultad en el mundo de la arena. Esto se debe que el robot tiene que moverse por una superficie con más fricción y con más irregularidades, lo que hace que las ruedas tengan que hacer un mayor esfuerzo para avanzar. 

![vel_wheels_sand](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/63ccb960-391b-4249-8732-fec89b5c690b)


### Análisis del mundo del suelo 🏞️

En el mundo del suelo, el robot se mueve con más facilidad. Esto se debe a que el robot se mueve por una superficie más lisa y con menos irregularidades, lo que hace que las ruedas tengan que hacer menos esfuerzo para avanzar. 

![vel_wheels_floor](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/d2674c61-c4bd-49d8-9b1e-a5be51d9542c)


### Conclusiones de la comparativa de velocidades

En conclusión, el robot se mueve con más facilidad en el mundo del suelo que en el mundo de la arena. Aunque no se aprecia mucho en la gráfica, se puede ver como tiene ligeramente menos oscilaciones en el suelo. En ambos mundos se puede apreciar bien cuando el robot choca con la caja, ya que la velocidad de las ruedas disminuye considerablemente.

## Comparativa de la odometría y la velocidad
![velpos_sand](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/bba741e3-9a38-4f2b-9153-725826105b8f)

![velpos_floor](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/580f0328-1258-4398-9c03-200081464e48)


Segun los resultados obtenidos, se puede apreciar que la odometría de la posición del robot es muy similar en ambos mundos.

## Comparativa de las aceleraciones 
### (Comparativa choque con caja)

Con las graficas de las aceleraciones se puede apreciar como el robot choca con la caja y como la aceleración aumenta considerablemente.

En el mundo del suelo, la aceleración aumenta de forma más brusca que en el mundo de la arena. Esto se debe a que en el mundo del suelo el robot se mueve con más facilidad, por lo que cuando choca con la caja, la aceleración aumenta de forma más brusca. Podemos apreciar dos grandes picos, uno cuando choca con las rueas delanteras y otro cuando choca con las ruedas traseras.

![aceleracion_floor](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/84fa5df8-5226-438a-9c5a-903027ad7099)

Al igual que en el mundo del suelo, en el mundo de la arena la aceleración aumenta de forma brusca cuando el robot choca con la caja. Auqnue en este caso, la aceleración aumenta de forma más suave que en el mundo del suelo. Podemos apreciar dos picos, uno cuando choca con las ruedas delanteras y otro cuando choca con las ruedas traseras.

![aceleracion_sand](https://github.com/jfisher2021/MyRover_Gazebo/assets/113594937/dd783ca7-5eab-4886-98f0-666e862d9b04)

### Conclusiones de la comparativa de las aceleraciones

En conclusión, el robot se mueve con más facilidad en el mundo del suelo que en el mundo de la arena. Aunque no se aprecia del todo en algunas gráficas, se puede ver como el robot tiene menos oscilaciones en el suelo. En ambos mundos se puede apreciar bien cuando el robot choca con la caja, ya que la velocidad de las ruedas disminuye considerablemente. Además, se puede apreciar como la aceleración aumenta de forma brusca cuando el robot choca con la caja en ambos mundos.

## Conclusiones finales 📝

Con esta practica hemos aprendido a visualizar el robot en rviz y simularlo en gazebo. Con la ayuda de los mundos proporcionados por el profesor, y de algunos launchers, hemos podido estudiar el comportamiento del robot en dos entornos distintos. 

## Datos adjuntos 📚

Los datos adjuntos se pueden encontrar en la carpeta [data](data). En esta carpeta se encuentran los datos de la simulación en formato rosbag.

