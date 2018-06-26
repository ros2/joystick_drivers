^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package joy
^^^^^^^^^^^^^^^^^^^^^^^^^

2.1.0 (2018-06-26)
------------------
* fixups after rebase
* 2.0.0
* Fix joystick driver to work with recent changes to rclcpp::Time. (`#4 <https://github.com/ros2/joystick_drivers/issues/4>`_)
  * Fix joystick driver to work with recent changes to rclcpp::Time.
  Signed-off-by: Chris Lalancette <clalancette@osrfoundation.org>
  * Move creation of time source outside of loop.
  Signed-off-by: Chris Lalancette <clalancette@osrfoundation.org>
* Switch to using the RCUTILS\_* macros. (`#3 <https://github.com/ros2/joystick_drivers/issues/3>`_)
  Signed-off-by: Chris Lalancette <clalancette@openrobotics.org>
* Move from bin to lib. (`#2 <https://github.com/ros2/joystick_drivers/issues/2>`_)
  Signed-off-by: Chris Lalancette <clalancette@openrobotics.org>
* set autorepeat_rate to 20
* remove ROS1 specific files
  convert package.xml and cmakelists to ROS2
  update licence
  don't lint ported code (and dont make it comply with pedantic)
  comment out diagnostic code and replace ROS1 calls with ROS2 calls
  use a rclcpp wall timer rather than a sleep
  fail if joystick.h not found
* rename joy_node to joy_node_linux
* Merge pull request `#108 <https://github.com/ros2/joystick_drivers/issues/108>`_ from hdino/indigo-devel
  Added dev_name parameter to select joystick by name
* Changed layout of README.md
* Added Readme for joy package with description of new device name parameter
* Added dev_name parameter to select joystick by name
* Merge pull request `#107 <https://github.com/ros2/joystick_drivers/issues/107>`_ from honeybee-robotics-forks/indigo-devel
  Numerous outstanding PRs.
* Added sticky buttons
  Fixed indentation
  Added CI config
  Added sticky buttons
  Changed messaging to better reflect what the script is doing
  Added wiimote testing script
  Changed package xml to format 2
  Fixed Indentation
  Fixed indentation
  Cleaned up code block
  Editted argc check block
  Removed unnesesary comment
  Formatted conditional block
  Fixed small errors
  Added comments decribing variables
  Removed blank line
  Fixed formatting issues
  Add CI config.
  Add CI config.
* Fixed issue when the joystick data did not got send until changed.
  Made the fix to error 91 an optional behavior
* Changed package xml to format 2
* Changed messaging to better reflect what the script is doing
  Fixed minor string error
  Changed tabs to spaces
* Contributors: Chris Lalancette, Dino Hüllmann, Jonathan Bohren, Mikael Arguedas, Miklos Marton, jprod123, psimona

1.11.0 (2017-02-10)
-------------------
* fixed joy/Cmakelists for osx
* Update dependencies to remove warnings
* Contributors: Marynel Vazquez, Mark D Horn

1.10.1 (2015-05-24)
-------------------
* Remove stray architechture_independent flags
* Contributors: Jonathan Bohren, Scott K Logan

1.10.0 (2014-06-26)
-------------------
* First indigo release
