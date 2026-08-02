# guided-radar-mapping-system
Embedded radar system using Arduino that scans and maps surrounding environment.

Collaborators: Ethan Chen, Jason Shi
<br><br>
<img width="549" height="800" alt="image" src="https://github.com/user-attachments/assets/2b0705d6-46d6-49e1-a26b-3dd22d91d21b" />
<img width="620" height="837" alt="image" src="https://github.com/user-attachments/assets/1893c375-1985-43ff-85f6-1a0f04b40c2c" />
<br><br>
- Joystick controls ultrasonic sensor mounted to a servo with low-latency positioning response. 
- 2D visualization tool built in Processing using Java receives Arduino sensor serial data and renders a polar radar map displaying object shapes and distance in real time.
- Two control modes: panning (joystick angle moves sensor either to the left or right) and tracking (joystick directly aims sensor, which matches joystick angle in real time)
- LCD display outputs current mode and distance detected. Buzzer sounds if distance is in danger threshold.
- Modes are toggled via button, LEDs indicate which mode is currently active.

## Arduino module
[Arduino code](sensorcontrols.ino)

## Polar display module
[Polar display code](polardisplay.pde)

