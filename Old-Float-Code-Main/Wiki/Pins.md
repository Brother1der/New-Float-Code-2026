# Pins

## Pins Definition
 
| Pin Name |  Pin Type | Definition |
| -------- | --------- | ---------- |
| VIN | Voltage Input Pin | Able to take in a voltage between 5.5V - 24V|
| IN A | Motor Direction Pin | When powered turn Motor turns Clockwise. Inputted from the Brain Controller. For Example the Arduino Controller will divert power into IN A|
| IN B | Motor Direction Pin | Inputted from the Brain Controller. For Example the Arduino Controller will divert power into IN B|
| VOUT | Outputed Voltage Pin | Delivers power to the motormaking sure that it runs consistantly|
| OUT A | Outputed Voltage Pin | The Pin which directly connects to the motor.  When powered turn Motor turns Clockwise.|
| OUT B | Outputed Voltage Pin | The Pin which directly connects to the motor.  When powered turn Motor turns Counter-Clockwise.| 
| EN A / DIAG A | Diagnostic Pin | Allows for a port to diagnose any issues with the board as well as to disable a the A sections of the board|
| EN B / DIAG B | Diagnostic Pin | Allows for a port to diagnose any issues with the board as well as to disable a the B sections of the board|
| PWM | Pulse Width Modulation Port| A volatage regulator which regulates the scale of different process of the motor controller e.x. it's value affects the RPM of the motor|
| GND | Ground Port | Regulates the Voltage of a circuit. Serves as a failsafe in case of a spike in voltage. Necessary to complete a circuit|
| GND A | Ground Port | Regulates the Voltage of the inputted volatge on the A Port. Serves as a failsafe in case of a spike in voltage. Necessary to complete a circuit|
| GND B | Ground Port | Regulates the Voltage of the inputted volatge on the B Port. Serves as a failsafe in case of a spike in voltage. Necessary to complete a circuit|
