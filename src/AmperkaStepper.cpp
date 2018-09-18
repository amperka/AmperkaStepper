#include "AmperkaStepper.h"

AmperkaStepper::AmperkaStepper(int numberSteps, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
  // which step the motor is on
  this->stepNumber = 0;      
  // the motor speed, in revolutions per minute
  this->speed = 0;
  // motor direction
  this->direction = 0;
  // time stamp in ms of the last step taken
  this->lastStepTime = 0;
  // total number of steps for this motor
  this->numberSteps = numberSteps;
  
  // Arduino pins for the motor control connection
  this->_pin1 = pin1;
  this->_pin2 = pin2;
  this->_pin3 = pin3;
  this->_pin4 = pin4;

  // setup the pins on the microcontroller
  pinMode(this->_pin1, OUTPUT);
  pinMode(this->_pin2, OUTPUT);
  pinMode(this->_pin3, OUTPUT);
  pinMode(this->_pin4, OUTPUT);
}

/*
  Sets the speed in revs per minute

*/
void AmperkaStepper::setSpeed(long revsPerMinute) {
  this->stepDelay = 60L * 1000L / this->numberSteps / revsPerMinute;
}

/*
  Moves the motor stepsToMove steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
void AmperkaStepper::step(int stepsToMove, uint8_t stepType) {  
  // how many steps to take
  int stepsLeft = abs(stepsToMove);
  
  // determine direction based on whether steps_to_mode is + or -
  if (stepsToMove > 0) {
    this->direction = 1;
  }
  if (stepsToMove < 0) {
    this->direction = 0;
  }

  if (stepType == FULL_STEP) {
    digitalWrite(this->_pin2, HIGH);
    digitalWrite(this->_pin3, HIGH);
  }

  unsigned long stepDelay = this->stepDelay;
  if (stepType == HALF_STEP) {
    stepDelay /= 2;
  }
    
  // decrement the number of steps, moving one step each time
  while(stepsLeft > 0) {

    // move only if the appropriate delay has passed
    if (millis() - this->lastStepTime >= stepDelay) {
      // get the timeStamp of when you stepped
      this->lastStepTime = millis();
      // increment or decrement the step number
      // depending on direction
      if (this->direction == 1) {
        this->stepNumber++;
        if (this->stepNumber == this->numberSteps) {
          this->stepNumber = 0;
        }
      } else { 
        if (this->stepNumber == 0) {
          this->stepNumber = this->numberSteps;
        }
        this->stepNumber--;
      }
      // decrement the steps left:
      stepsLeft--;

      // WAVE_DRIVE, FULL_STEP, HALF_STEP 
      if (stepType == HALF_STEP) {
        // step the motor to step number 0, 1, 2, 3, 4, 5, 6, 7, 8
        stepMotor(this->stepNumber % 8, stepType);
      }
      else {
        // step the motor to step number 0, 1, 2, or 3
        stepMotor(this->stepNumber % 4, stepType);
      }
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void AmperkaStepper::stepMotor(int thisStep, uint8_t stepType) {
  if (stepType == FULL_STEP) {
    switch (thisStep) {
      // 01
      case 3:
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin4, HIGH);
      break;
      // 11
      case 2:
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin4, HIGH);
      break;
      // 10
      case 1:
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin4, LOW);
      break;
      // 00
      case 0:
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin4, LOW);
      break;
    } 
  } else if (stepType == WAVE_DRIVE) {
    switch (thisStep) {
      // 1010
      case 0:
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin3, LOW);
      digitalWrite(_pin4, HIGH);
      break;
      // 0110
      case 1:
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin2, LOW);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, HIGH);
      break;
      // 0101
      case 2:
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin3, LOW);
      digitalWrite(_pin4, HIGH);
      break;
      // 1001
      case 3:
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin2, LOW);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, LOW);
      break;
    } 
  } else if (stepType == HALF_STEP) {
    switch (thisStep) {
      // 1, 1, 0, 0
      case 0:
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin3, LOW);
      digitalWrite(_pin4, LOW);
      break;
      // 1, 1, 1, 1
      case 1:
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, HIGH);
      break;
      // 0, 0, 1, 1
      case 2:
      digitalWrite(_pin2, LOW);
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, HIGH);
      break;
      // 1, 0, 1, 1
      case 3:
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, HIGH);
      break;
      // 1, 0, 0, 0
      case 4:
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin3, LOW);
      digitalWrite(_pin4, LOW);
      break;
      // 1, 0, 1, 0
      case 5:
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, LOW);
      break;
      // 0, 0, 1, 0
      case 6:
      digitalWrite(_pin2, LOW);
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, LOW);
      break;
      // 1, 1, 1, 0
      case 7:
      digitalWrite(_pin2, HIGH);
      digitalWrite(_pin1, HIGH);
      digitalWrite(_pin3, HIGH);
      digitalWrite(_pin4, LOW);
      break;
    }
  }
}
