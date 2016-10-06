#define RECV_PIN 10
#define MIN_RECEIVE 200

#include "ir_buttons.h"
//
// translates button presees from infrared source and sends to commander
//
class InfraredComms {
private:
  long           lastReceive;
  IRrecv         irrecv;
  decode_results results;
  Commander*     cmdr;
  String         curEntry;

public:
  InfraredComms(Commander* cdr) :irrecv(RECV_PIN) {
    cmdr = cdr;
  }
  
  void setup() {
    irrecv.enableIRIn(); // Start the receiver
  }

  void appendDigit(String num) {
      if (curEntry.length() < 4) {
        curEntry += num;
      }
  }

  void processIRCommands() {
    if (irrecv.decode(&results)) {
      long curReceive = millis();
      if (curReceive - lastReceive > MIN_RECEIVE) {
        lastReceive = curReceive;
        long button = results.value & 2047; //
      
        switch(button) {
        case OK_BUTTON:       cmdr->cmdKeepGoing(); break;
        case UP_BUTTON:       cmdr->cmdIncrease(); break;
        case DOWN_BUTTON:     cmdr->cmdDecrease(); break;
        case LEFT_BUTTON:     cmdr->cmdOff();      break;
        case RIGHT_BUTTON:    cmdr->cmdOn();       break;
        case VOL_UP_BUTTON:   break;            
        case VOL_DOWN_BUTTON: cmdr->cmdSetMin();   break;
        case ZERO_BUTTON:    appendDigit("0");    break;
        case ONE_BUTTON:      appendDigit("1");            break;
        case TWO_BUTTON:      appendDigit("2");            break;
        case THREE_BUTTON:    appendDigit("3");            break;
        case FOUR_BUTTON:     appendDigit("4");           break;
        case FIVE_BUTTON:     appendDigit("5");            break;
        case SIX_BUTTON:      appendDigit("6");            break;
        case SEVEN_BUTTON:    appendDigit("7");            break;
        case EIGHT_BUTTON:    appendDigit("8");            break;
        case NINE_BUTTON:     appendDigit("9");            break;
        case STAR_BUTTON:     break;
        case POUND_BUTTON:    cmdr->tryUnlock(curEntry); curEntry = "";        break;
        default:
          if (debug) Serial.print("unknown button:");
          if (debug) Serial.println(button);
          break;
        }
      }
      irrecv.resume();
    }
  }
};
