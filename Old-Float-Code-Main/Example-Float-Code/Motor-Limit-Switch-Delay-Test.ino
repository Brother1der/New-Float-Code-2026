#include <ezButton.h>

// Define motor control pins
const int voltA = 5;
const int voltB = 11;
const int pwm_port = 9;
const int duty_cycle = 255;  

// Define switch pins
const int top_switch_pin = 12;
const int bottom_switch_pin = A3;

// Initialize switches with debounce
ezButton top_switch(top_switch_pin);
ezButton bottom_switch(bottom_switch_pin);

// Timers for submerging and surfacing
const unsigned long WAIT_TIME = 5000; // Wait for 5 secs
unsigned long state_timer = 0;

// Enum for motor direction
enum Motor_Direction {
  CLOCKWISE,
  COUNTERCLOCKWISE,
  STALLED
};

Motor_Direction motor_direction = STALLED; // Default motor state

// State Machine for Motor Operation
enum Motor_State {
  IDLE,
  MOVING_DOWN,
  WAITING_BOTTOM,
  MOVING_UP,
  WAITING_TOP
};

Motor_State motor_state = IDLE;

void setup() {
  // Configure switches with debouncing
  top_switch.setDebounceTime(50);
  bottom_switch.setDebounceTime(50);

  // Set motor control pins as outputs
  pinMode(voltA, OUTPUT);
  pinMode(voltB, OUTPUT);
  pinMode(pwm_port, OUTPUT);

  // Configure built-in LED for status indication
  pinMode(LED_BUILTIN, OUTPUT);

  // Set switch pins as inputs with pull-up resistors
  pinMode(top_switch_pin, INPUT_PULLUP);
  pinMode(bottom_switch_pin, INPUT_PULLUP);

  bottom_switch.loop(); // Ensure we get an initial state read

  // Initial motor setup: move down if not at the bottom
  if (!bottom_switch.isPressed()) {
    motor_state = MOVING_DOWN;
    motor_direction = CLOCKWISE;
  } else {
    motor_state = IDLE;
    motor_direction = STALLED;
  }

  controlMotor();
}

void loop() {
  // Update button states
  top_switch.loop();
  bottom_switch.loop();

  // Read switch states
  bool top_pressed = top_switch.isPressed();
  bool bottom_pressed = bottom_switch.isPressed();

  // Get current time for timing logic
  unsigned long current_time = millis();

  // State machine for motor operation
  switch (motor_state) {
    case IDLE:
      // if bottom switch is not pressed, start moving down
      if (!bottom_pressed) {
        motor_state = MOVING_DOWN;
        motor_direction = CLOCKWISE;
      }
      break;

    case MOVING_DOWN:
      // stop when the bottom switch is pressed
      if (bottom_pressed) {
        motor_state = WAITING_BOTTOM;
        motor_direction = STALLED;
        state_timer = current_time;
      }
      break;

    case WAITING_BOTTOM:
      // wait 5 seconds before moving up
      if (current_time - state_timer >= WAIT_TIME) {
        motor_state = MOVING_UP;
        motor_direction = COUNTERCLOCKWISE;
      }
      break;

    case MOVING_UP:
      // stop when the top switch is pressed
      if (top_pressed) {
        motor_state = WAITING_TOP;
        motor_direction = STALLED;
        state_timer = current_time;
      }
      break;

    case WAITING_TOP:
      // wait 5 seconds before moving down
      if (current_time - state_timer >= WAIT_TIME) {
        motor_state = MOVING_DOWN;
        motor_direction = CLOCKWISE;
      }
      break;
  }

  // Control the motor based on the direction
  controlMotor();
}

void controlMotor() {
  switch (motor_direction) {
    case CLOCKWISE:
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(voltA, HIGH);
      digitalWrite(voltB, LOW);
      analogWrite(pwm_port, duty_cycle); 
      break;

    case COUNTERCLOCKWISE:
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(voltA, LOW);
      digitalWrite(voltB, HIGH);
      analogWrite(pwm_port, duty_cycle);
      break;

    case STALLED:
    default:
      digitalWrite(voltA, LOW);
      digitalWrite(voltB, LOW);
      analogWrite(pwm_port, 0); 
      break;
  }
}
