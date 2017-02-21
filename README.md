# AVRServo
Simple digital servo driver for the ATtiny85.  
Drop in replacement for the steering DC motor in small RC cars with an SG90 micro servo.

Inputs:  
PB3, PB4 - Vcc referenced single ended analog inputs for left/right position limit  
PB1, PB2 - Direction control

Outputs:  
PB0 - PWM signal

### Operation:  
The steering motor drive wires an connected directly to direction control. Analog inputs are connected to a potmeter between Vcc and GND. At startup the servo is centered, when direction is changed on the remote the servo is position is set according to the potmeters for left and right side respectively.

PWM signal is generated with simple busy loop, delays are calibrated to individual servos.

