#include <ezButton.h>

// Define motor control pins
const int voltA = 5;
const int voltB = 11;
const int pwm_port = 9;
const int duty_cycle = 255; // Maximum speed (0-255)

// Define switch pins
const int topSwitchPin = 12;
const int bottomSwitchPin = A3;

// Initialize switches with debounce
ezButton topSwitch(topSwitchPin);
ezButton bottomSwitch(bottomSwitchPin);

// Enum for motor direction
enum Motor_Direction {
  CLOCKWISE,
  COUNTERCLOCKWISE,
  STALLED
};

// Variable to track motor direction
Motor_Direction motor_direction = STALLED;

void setup() {
  Serial.begin(9600);
  
  // Configure switches with debouncing
  topSwitch.setDebounceTime(50);
  bottomSwitch.setDebounceTime(50);

  // Set motor control pins as outputs
  pinMode(voltA, OUTPUT);
  pinMode(voltB, OUTPUT);
  pinMode(pwm_port, OUTPUT);

  // Configure built-in LED for status indication
  pinMode(LED_BUILTIN, OUTPUT);

  // Set switch pins as inputs with pull-up resistors
  pinMode(topSwitchPin, INPUT_PULLUP);
  pinMode(bottomSwitchPin, INPUT_PULLUP);
}

void loop() {
  // Update switch states (handles debounce)
  topSwitch.loop();
  bottomSwitch.loop();

  // Read switch states
  bool topPressed = topSwitch.isPressed();
  bool bottomPressed = bottomSwitch.isPressed();

  // Change motor direction only when a button is pressed
  if (topPressed && bottomPressed) {
    motor_direction = STALLED; // Stop motor if both are pressed
  } else if (topPressed) {
    motor_direction = CLOCKWISE;
  } else if (bottomPressed) {
    motor_direction = COUNTERCLOCKWISE;
  }

  // Control motor direction
  switch (motor_direction) {
    case CLOCKWISE:
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(voltA, HIGH);
      digitalWrite(voltB, LOW);
      analogWrite(pwm_port, duty_cycle); // Apply PWM
      break;
    
    case COUNTERCLOCKWISE:
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(voltA, LOW);
      digitalWrite(voltB, HIGH);
      analogWrite(pwm_port, duty_cycle); // Apply PWM
      break;
    
    case STALLED:
    default:
      digitalWrite(voltA, LOW);
      digitalWrite(voltB, LOW);
      analogWrite(pwm_port, 0); // Stop motor
      break;
  }
}
