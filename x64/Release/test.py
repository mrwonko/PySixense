import sys
#print(sys.version)
#print(sys.path)
#sys.path.append(".")

import PySixense

# init
assert(PySixense.Init() == PySixense.Constants.Success)

# bases are not immediately available
print("Waiting a second for Initialization to finish...")
import time
time.sleep(1)

# query active bases
connectedBases = []
for i in range(PySixense.GetMaxBases()):
    if PySixense.IsBaseConnected(i):
        connectedBases.append(i)
        print("Base {} is connected!".format(i))

# abort if no bases available
if len(connectedBases) == 0:
    print("No connected bases found!")
    assert(PySixense.Exit() == PySixense.Constants.Success)
    import sys
    sys.exit()

PySixense.SetActiveBase(connectedBases[0])

enabledControllers = []
for i in range(PySixense.GetMaxControllers()):
    if PySixense.IsControllerEnabled(i):
        enabledControllers.append(i)
        print("Controller {} is enabled!".format(i))

for i in enabledControllers:
    success, data = PySixense.GetNewestData(i)
    if success == PySixense.Constants.Failure:
        print("Could not get data of controller {}!".format(i))
    else:
        print("""Controller {i} data:
    Position: {data.pos}
    Rotation Matrix:
        {data.rot_mat[0]}
        {data.rot_mat[1]}
        {data.rot_mat[2]}
    Rotation Quaternion:
        {data.rot_quat}
    joystick x: {data.joystick_x}
    joystick y: {data.joystick_y}
    trigger: {data.trigger}
    buttons: {data.buttons}
    sequence number: {data.sequence_number}
    firmware revision: {data.firmware_revision}
    hardware revision: {data.hardware_revision}
    packet type: {data.packet_type}
    magnetic frequency: {data.magnetic_frequency}
    enabled: {data.enabled}
    index: {data.controller_index}
    docked: {data.is_docked}
    hand: {data.which_hand}
""".format(i = i, data=data))

print("We're done here. Bye bye!")
assert(PySixense.Exit() == PySixense.Constants.Success)
print("dead")
