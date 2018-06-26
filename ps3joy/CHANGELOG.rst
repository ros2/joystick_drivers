^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package ps3joy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2.1.0 (2018-06-26)
------------------
* 2.0.0
* change package to format2
  drop ament_ignores in all packages but joy
* Merge pull request `#110 <https://github.com/ros2/joystick_drivers/issues/110>`_ from honeybee-robotics-forks/indigo-devel
  Adding additional documentation to ps3joy
* Update README.md
* Merge pull request `#17 <https://github.com/ros2/joystick_drivers/issues/17>`_ from alab288/indigo-devel
  Added and updated files for ps3joy
* Update README.md
* Command line Options moved to README.md
* Added and updated files for ps3joy
* Merge pull request `#107 <https://github.com/ros2/joystick_drivers/issues/107>`_ from honeybee-robotics-forks/indigo-devel
  Numerous outstanding PRs.
* Merge pull request `#10 <https://github.com/ros2/joystick_drivers/issues/10>`_ from alab288/patch-4
  Update testing.md
* Update testing.md
* Create bluetooth_devices.md
* Update README.md
* Update README.md
* Rename ps3joy/testing.md to ps3joy/doc/testing.md
* Rename procedure_test.md to testing.md
* Merge pull request `#8 <https://github.com/ros2/joystick_drivers/issues/8>`_ from alab288/patch-3
  Created testing guide for ps3joy.
* Create procedure_test.md
* Delete procedure_test.md
* Created ps3joy test procedures
* Delete mainpage.dox
* Updated the README.md
  cleaned up the format and syntax
* Rename README to README.md
* root notice reinstalled
  Refined root notice
* Let ps3joy_node not quit on inactivity-timeout.
  I don't know why this was in there, now reconnecting the controller after
  inactivity-timeout works fine just like with the normal ps3joy.py
* Refine diagnostics message usage in ps3joy_node
* Improve ps3joy_node with rospy.init_node and .is_shutdown
  Replace unused self.shutdown with rospy.is_shutdown()
  Call rospy.init_node with anonymous=False because the hardware cannot be accessed multiple times concurrently
* Remove quit on failed root level check, part one of issue `#53 <https://github.com/ros2/joystick_drivers/issues/53>`_
* Create README
  Information about the bluez package bluez-5.37 was added.
  Information about restarting bluez was also added.
* Changed package xml to format 2
* Contributors: Alenso Labady, Felix Kolbe, Jonathan Bohren, Mikael Arguedas, alab288, jprod123

1.11.0 (2017-02-10)
-------------------
* Update dependencies to remove warnings
* Contributors: Mark D Horn

1.10.1 (2015-05-24)
-------------------
* Remove stray architechture_independent flags
* Contributors: Jonathan Bohren, Scott K Logan

1.10.0 (2014-06-26)
-------------------
* First indigo reelase
* Update ps3joy/package.xml URLs with github user ros to ros-drivers
* Prompt for sudo password when required
* Contributors: Felix Kolbe, Jonathan Bohren, dawonn
