# PySixense - A Python Binding of the Sixense SDK

## Overview

The PySixense.pyd file is the Python Module you'll want to import:

```Python
import PySixense
```

In general, your code will look somewhat like this:

```Python
import PySixense

if PySixense.Init() != PySixense.Constants.Success:
	print("Error initializing Sixense SDK")
else:
	# code goes here
	# ...
	
	# exit properly
	assert(PySixense.Deinit() == PySixense.Constants.Success)
```

## Structures

### PySixense.ControllerData

When you query controller data using GetData(), GetAllData(), GetNewestData() or GetAllNewestData() you get the data as this structure. Here's what it contains:

* *pos* - list of 3 numbers describing the controller's 3D position (X, Y, Z)
* *rot_mat* - A list of 3 lists of 3 numbers each representing the matrix describing the controller's rotation
* *joystick_x* - The analogue stick's horizontal position, with -1 being left, 0 the center and 1 right
* *joystick_y* - The analogue stick's vertical position, with -1 being down, 0 the center and 1 up
* *trigger* - The position of the analogue trigger, with 0 being unpressed and 1 fully pressed
* *buttons* - Bitflags containing the state of each button, do a bitwise and with the corresponding button mask to retrieve the state:
  
```Python
if data.buttons & PySixense.Constants.Button1:
	print("Button 1 pressed!")
```
  
  There are the following button constants:
  - PySixense.Constants.Button1
  - PySixense.Constants.Button2
  - PySixense.Constants.Button3
  - PySixense.Constants.Button4
  - PySixense.Constants.ButtonStart
  - PySixense.Constants.ButtonBumper
  - PySixense.Constants.ButtonJoystick
* *sequence_number* - The read values are numbered from 0 - 255, which each subsequent datapoint getting one more. (The update rate is 60Hz.)
* *rot_quat* - List of 4 numbers representing a quaternion describing the controller's rotation
* *firmware_revision* - Current firmware revision
* *hardware_revision* - Current hardware revision
* *packet_type* - Type of data packet, currently always 1
* *magnetic_frequency* - Unused
* *enabled* - Whether this controller is enabled (True or False)
* *controller_index* - Hardware index of the controller this data packet is for
* *is_docked* - Whether the controller is currently in the dock (True or False)
* *which_hand* - The first time the controller is docket, the hand gets set based on the side used. 0 = not docket yet, 1 = left, 2 = right

## Constants

PySixense.Constants contains a couple of constants used - the Button Bitmasks listed above and the following:

* *PySixense.Constants.Success* - Returned when functions succeed
* *PySixense.Constants.Failure* - Returned when functions fail

Note that not all functions return their status.

## Functions

### PySixense.SixenseInit()

Initializes the SDK. Must be successfully called before using any other function.

Returns status.

### PySixense.SixenseExit()

Must be called once you're done with the SDK if you called SixenseInit()

Returns status.

### PySixense.GetMaxBases()

Returns the maximum number of supported bases, currently 4.

### PySixense.SetActiveBase(base_num)

Sets the base of the given index active (i.e. all base-related functions calls will apply to this one)

Returns status

### PySixense.IsBaseConnected(base_num)

Returns whether the base of the given index is currently connected to the PC. (0 or 1)

### PySixense.GetMaxControllers()

Returns the maximum number of controllers supported (per base?)

### PySixense.GetNumActiveControllers()

Returns the number of active controlles of the active base. Use IsControllerEnabled() to find out which ones are the active ones.

### PySixense.IsControlledEnabled(controller_index)

Returns whether the controller of the given index is enabled (0 or 1)

### PySixense.GetAllNewestData()

Returns status and a list containing 4 PySixense.ControllerData (one for each PySixense.GetMaxControllers()) with the newest data.

### PySixense.GetAllData(index_back)

Returns status and a list containing 4 PySixense.ControllerData (one for each PySixense.GetMaxControllers()) with the data from index_back polls ago (in range [0, PySixense.GetHistorySize() - 1])

### PySixense.GetNewestData(controller_index)

Returns status and a PySixense.ControllerData with the newest data for the given controller

### PySixense.GetData(controller_index, index_back)

Returns status and a PySixense.ControllerData with the data from index_back polls ago (in range [0, PySixense.GetHistorySize() - 1]) for the given controller

### PySixense.GetHistorySize()

How much previous ControllerData is available - usually 10.

### PySixense.SetFilterEnabled(on_or_off)

Enables or disabled filtering. (i.e. mapping movements to a certain range)

Returns success.

### PySixense.GetFilterEnabled()

Returns status and whether filtering is enabled (True or False)

### PySixense.SetFilterParams(near_range, near_val, far_range, far_val)

Please refer to the Sixense SDK Reference

### PySixense.TriggerVibration(controller_id, duration_100ms, pattern_id)

On controllers that support it (not the Razer Hydra), this triggers a vibration lasting duration_100ms deciseconds. pattern_id is currently ignored.

Returns status.

### PySixense.AutoEnableHemisphereTracking(controller_id)

Deprecated, refer to the Sixense SDK Reference if you really need it.

### PySixense.SetHighPriorityBindingEnabled(on_or_off)

Only for wireless devices. Refer to the Sixense SDK Reference if you need it.

### PySixense.GetHighPriorityBindingEnabled()

Only for wireless devices. Refer to the Sixense SDK Reference if you need it. Returns a 2-tuple.

### PySixense.SetBaseColor(red, green, blue)

Only available on the Wireless Devkits. Refer to the Sixense SDK Reference if you need it.

### PySixense.GetBaseColor()

Only available on the Wireless Devkits. Refer to the Sixense SDK Reference if you need it. Returns a 4-tuple