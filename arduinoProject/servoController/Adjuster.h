#define minPos 18.0
#define maxPos 180.0
#define servoPin 14
#define increment (maxPos-minPos)/100.0

//
// adjusts control servo up and down to control output level
//
class Adjuster {
private:  
  Servo controlServo;
  float pos;
  bool on;
  LEDDisplay* led_disp;
  bool doAck;
  
public:  
    Adjuster (LEDDisplay* disp) {
    pos = minPos;
    on = false;
    led_disp = disp;
    doAck = false;
  }

  void enableAck() {
    doAck = true;
  }
  
  void turnOn(bool state) { 
    // turning on
    if (!on && state ) {
      on = true;
      if (doAck) {
        Serial.println("on:1");
      }
    }
    
    // turning off
    if (on && !state)
    {
      on = false;
      if (doAck) {
        Serial.println("on:0");
      }
    }
    updateDisplay();    
  }
  
  void updateDisplay() {
    led_disp->updateDisplay(getPercent(), on);
  }
  
  int getPercent() {
    return (float)(pos - minPos) / (float)(maxPos - minPos) * 100.0;
  }
  
  float getPos() {
    return pos; 
  }
  
  bool isOn() {
    return on;
  }
  
  void setup() {
    controlServo.attach(servoPin);
    controlServo.write((int)minPos);            
  }
  
  void doIncrement() {
    if (on && pos < maxPos) {
      pos = pos + increment;
      moveToCurrentPosition();
    }
  }
  
  void doDecrement() {
    if (on && pos > minPos) {
      pos = pos - increment;
      moveToCurrentPosition();
    }
  }
  
  void setPosition(int numIncrements) {
    pos = minPos + (increment * numIncrements);
    moveToCurrentPosition();
  }  
  
  void moveToCurrentPosition() {
      if (pos > maxPos) {
        pos = maxPos;
      }
      if (pos < minPos) {
        pos = minPos;
      }
      
      updateDisplay();
      
      controlServo.write((int)pos);
      if (doAck) {
        Serial.print("pos:");
        Serial.println(getPercent());
      }
  }
};
