#define COMMAND_TIMEOUT 60000
#define ACCESS_CODE "1233"

//
// interprets commands and performs by calls to display and adjuster
//
class Commander {
private:
  bool unlocked;
  bool doAck;

  long commandTime;
  
  LEDDisplay* led_disp;
  Adjuster*   adjuster;
  
public:
  Commander(LEDDisplay* disp, Adjuster* adj) {
    unlocked = false;
    commandTime = 0;
    led_disp = disp;
    adjuster = adj;
    doAck = false;
  }
  
  void enableAck() {
    doAck = true;
  }

  void setup() {
  }

  bool isUnlocked() {
    return unlocked;
  }
  
  void reportLockStatus() {
    if (doAck) {
      if (unlocked)
        Serial.println("locked:false");
      else  
        Serial.println("locked:true");      
    }
  }
  
  void cmdKeepGoing() {
    reportLockStatus();
    gotCommand();
  }
  
  void cmdIncrease() {
    if (unlocked) {
      adjuster->doIncrement();
      gotCommand();
    }
  }
  
  void cmdDecrease() {
    if (unlocked) {
      adjuster->doDecrement();
      gotCommand();
    }
  }
  
  void cmdOff() {
    adjuster->turnOn(false);
    adjuster->setPosition(0);
    gotCommand();
  }
  
  void cmdOn() {
    if (unlocked) {
      adjuster->turnOn(true);
      gotCommand();  
    }
  }
  
  void cmdSetMin() {
    if (unlocked) {
      adjuster->setPosition(0);          
      gotCommand();
    }
  }
  
  void checkTimeout() {
    if (::millis() - commandTime > COMMAND_TIMEOUT) {
      if (adjuster->isOn()) {
        cmdOff();      
        if (doAck) {
          Serial.println("timeout");
        }
      }
    }
  }

  void cmdUnlock() {
    unlocked = true;    
    reportLockStatus();
    adjuster->updateDisplay();
    gotCommand();
  }

  void gotCommand() {
    commandTime = ::millis();
  }

  void tryUnlock(String entry) {
    if (!unlocked) {
      if (entry == ACCESS_CODE) {
        cmdUnlock();
      } else {
        if (doAck) {
          Serial.print("unlockfail:");
          Serial.println(entry.c_str());
        }
        char tempString[10]; 
        sprintf(tempString, "   -");    
        led_disp->print(tempString); 
        delay(300);
        sprintf(tempString, "    ");    
        led_disp->print(tempString); 
      }    
    } else {
      reportLockStatus();
    }
  }   
};
