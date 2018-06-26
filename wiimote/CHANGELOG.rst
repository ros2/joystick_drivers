^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package wiimote
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2.1.0 (2018-06-26)
------------------
* 2.0.0
* change package to format2
  drop ament_ignores in all packages but joy
* Merge pull request `#107 <https://github.com/ros2/joystick_drivers/issues/107>`_ from honeybee-robotics-forks/indigo-devel
  Numerous outstanding PRs.
* Merge pull request `#9 <https://github.com/ros2/joystick_drivers/issues/9>`_ from jprod123/patch-2
  Fixed issue when nunchuk wasn't connected
* Update README.md
* Update README.md
* Rename wiimote/TELEOP.md to wiimote/doc/tutorials/teleop.md
* Rename wiimote/testing_proceedures.md to wiimote/doc/testing.md
* Merge pull request `#12 <https://github.com/ros2/joystick_drivers/issues/12>`_ from jprod123/patch-4
  Added testing proceedures for wiimote
* Merge pull request `#11 <https://github.com/ros2/joystick_drivers/issues/11>`_ from jprod123/patch-3
  Resolves div-by-zero error race condition on startup.
* Added testing proceedures for wiimote
  Added documentation on the wiimote package and how to use it.
* Avoids the divide by zero warning
  Adds a check to ensure the calibration values don't cause a divide by zero error
* Fixed issue when nunchuk wasn't connected
  Fixed issue where script would try to get calibration data from the nunchuk without checking if a nunchuk is connected
* Delete mainpage.dox
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
* Changed package xml to format 2
* Added wiimote testing script
* Changed messaging to better reflect what the script is doing
  Fixed minor string error
  Changed tabs to spaces
* Add pairing params to wiimote_node
  * bluetooth_addr
  * pair_timeout
* Contributors: Jonathan Bohren, Matt Vollrath, Mikael Arguedas, jprod123, psimona

1.11.0 (2017-02-10)
-------------------
* Sample Teleop Implementation for Wiimote
* C++ Implementation of Wiimote Controller Node
* Add queue_size to remove ROS Warning
* Update dependencies to remove warnings
* Contributors: Mark D Horn

1.10.1 (2015-05-24)
-------------------

1.10.0 (2014-06-26)
-------------------
* First indigo release
