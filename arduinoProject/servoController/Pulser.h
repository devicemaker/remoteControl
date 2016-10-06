#define MICROS_PER_PULSE 2000 
#define MILLIS_BETWEEN_CHANNEL 10 
#define NUM_PULSES 4

#define CH1_PIN1 2
#define CH1_PIN2 3

#define CH2_PIN1 4
#define CH2_PIN2 5

#define CH3_PIN1 6
#define CH3_PIN2 7

#define CH4_PIN1 8
#define CH4_PIN2 9

//
// produces output pulses
//
class Pulser {
private:  
  
public:
  void setup() {
    pinMode(CH1_PIN1, OUTPUT);     
    pinMode(CH1_PIN2, OUTPUT);     
    pinMode(CH2_PIN1, OUTPUT);     
    pinMode(CH2_PIN2, OUTPUT);     
    pinMode(CH3_PIN1, OUTPUT);     
    pinMode(CH3_PIN2, OUTPUT);     
    pinMode(CH4_PIN1, OUTPUT);     
    pinMode(CH4_PIN2, OUTPUT);     
  
  }
  
  void pulsePins(int pinA, int pinB) {
    for (int curPulse=0; curPulse < NUM_PULSES; curPulse++) {
        digitalWrite(pinA, HIGH);   
        digitalWrite(pinB, LOW);  
        delayMicroseconds(MICROS_PER_PULSE);
        digitalWrite(pinA, LOW);  
        digitalWrite(pinB, HIGH);   
        delayMicroseconds(MICROS_PER_PULSE);
    }
    // all pins off
    digitalWrite(pinA, LOW);    
    digitalWrite(pinB, LOW);    
    
    delay(MILLIS_BETWEEN_CHANNEL);
  }
  
  void pulseAllChannels() {
      pulsePins(CH1_PIN1, CH1_PIN2);
      pulsePins(CH2_PIN1, CH2_PIN2);
      pulsePins(CH3_PIN1, CH3_PIN2);
      pulsePins(CH4_PIN1, CH4_PIN2);
  }
};
