#define LED_TX_PIN 16
#define LED_RX_PIN 17

//
// display output level
//
class LEDDisplay {
private:
  SoftwareSerial led;

public:
  LEDDisplay () : led(LED_RX_PIN, LED_TX_PIN) {
  }
  
  void setup() {
    led.begin(9600);
    delay(1000);
    led.write('v');
    led.print("    ");
  
  }

  void print(char* tempstr) {
      led.print(tempstr); 
  }
  
  void updateDisplay(int percent, bool isOn) {
    char tempString[10];
    
    if (debug) {
      Serial.print("isOn:");
      Serial.println(isOn);
    
      Serial.print("percent:");
      Serial.println(percent);
    }
    
    if (isOn) {
      sprintf(tempString, " %03d", percent); 
    } else {
      sprintf(tempString, "-OFF");    
    }
    
    led.print(tempString); 
  }
};  
