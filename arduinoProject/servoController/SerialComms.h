#define ON_CMD         'O'
#define OFF_CMD        'F'
#define UNLOCK_CMD     'U'
#define INCREASE_CMD   'I'
#define DECREASE_CMD   'D'
#define MINIMUM_CMD    'M'
#define KEEP_GOING_CMD 'K'

//
// translates serial command messages and sends to commander
//
class SerialComms {
  Commander* cmdr;
  Adjuster*  adjuster;

public:  
  SerialComms(Commander* c, Adjuster* adj) : cmdr(c), adjuster(adj) {
  }

  void processSerialCommand() {
    String serialCmd;
    
    while (Serial.available() > 0)
    {
      char recieved = Serial.read();
          
      if (recieved == '\n')
      {
        char cmd_code = serialCmd[0];
        String cmd_param = serialCmd.substring(1);
        
        parseCommand(cmd_code, cmd_param);
        serialCmd = "";
      } else {
        serialCmd += recieved; 
      }
    }
  }
  

  void parseCommand(char cmd, String param) {
    adjuster->enableAck();    
    cmdr->enableAck();

    if (!cmdr->isUnlocked()) {
      nackCommand(cmd);
    } else {
      switch (cmd) {
      case ON_CMD:
        cmdr->cmdOn();      
        ackCommand(cmd);
        break;
      case OFF_CMD:
        cmdr->cmdOff();      
        ackCommand(cmd);
        break;
      case UNLOCK_CMD:
        cmdr->tryUnlock(param);
        ackCommand(cmd);
        break;
      case INCREASE_CMD:
        cmdr->cmdIncrease();      
        ackCommand(cmd);
        break;
      case DECREASE_CMD:
        cmdr->cmdDecrease();      
        ackCommand(cmd);
        break;
      case MINIMUM_CMD:
        cmdr->cmdSetMin();      
        ackCommand(cmd);
        break;
      case KEEP_GOING_CMD:
        cmdr->cmdKeepGoing();      
        ackCommand(cmd);
        break;
        
      default:
        nackCommand(cmd);
      }
    }
  }
  
  void ackCommand(char cmd) {
    Serial.print("ack:");
    Serial.println(cmd);
  }
  
  void nackCommand(char cmd) {
    Serial.print("nack:");
    Serial.println(cmd);
  }  
};
