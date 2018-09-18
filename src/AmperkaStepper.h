#ifndef _AMPERKA_STEPPER_H_
#define _AMPERKA_STEPPER_H_

#include <Arduino.h>

enum STEP_TYPE {
    WAVE_DRIVE,
    FULL_STEP,
    HALF_STEP
};

class AmperkaStepper {
public:
    AmperkaStepper(int numberSteps, uint8_t pin1 = 4, uint8_t pin2 = 5, uint8_t pin3 = 6, uint8_t pin4 = 7);
    // speed setter method
    void setSpeed(long revsPerMinute);
    // mover method
    void step(int numberSteps, uint8_t stepType = FULL_STEP);

protected:
    void stepMotor(int this_step, uint8_t stepType);
    // direction of rotation
    bool direction;
    // speed in RPMs
    int speed;
    // delay between steps, in ms, based on speed
    unsigned long stepDelay;
    // total number of steps this motor can take
    int numberSteps;
    // which step the motor is on
    int stepNumber;
    
    // motor pin numbers:
    uint8_t _pin1;
    uint8_t _pin2;
    uint8_t _pin3;
    uint8_t _pin4;
    
    // time stamp in ms of when the last step was taken
    long lastStepTime;
};

#endif  // _AMPERKA_STEPPER_H_

