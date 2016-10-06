#include <Servo.h>
#include <IRremote.h>
#include <SoftwareSerial.h>

bool debug = false;

#include "LEDDisplay.h"
#include "Adjuster.h"
#include "Pulser.h"
#include "Commander.h"
#include "InfraredComms.h"
#include "SerialComms.h"

LEDDisplay    disp;
Pulser        pulser;
Adjuster      adjuster(&disp);
Commander     cmdr(&disp,&adjuster);
InfraredComms irComms(&cmdr);
SerialComms   serComms(&cmdr, &adjuster);

void setup() {  
  Serial.begin(9600);  

  disp.setup();
  pulser.setup();
  adjuster.setup();
  irComms.setup();
}

void loop()                     
{
  // turn off from inactivity
  cmdr.checkTimeout();

  // look for infrared remote control commands
  irComms.processIRCommands();

  // look for serial port remote control commands
  serComms.processSerialCommand();

  // if turned on, then produce pulses
  if (adjuster.isOn()) {
    pulser.pulseAllChannels();
  }
}
