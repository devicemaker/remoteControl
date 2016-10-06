# remoteControl
there are two pieces of software in this project:

1. an arduino program to do the following:
  * Generate signal pulses out of 8 pins in pairs arranged as 4 channels
  * Rotate a servo to a specific position indicated as 0 to 100
  * Accept infrared remote control signals
  * Accept USB remote control signals
  * Indicate status on a 4 digit display
  * timeout and turn off if no commands for 60 seconds

2. A web server to do the following
  * connect to an arduino on a USB port running the above software
  * Accept websocket connections from a client page and authenticate
  * receive commands from the websocket client and forward them to the arduino
  * receive status from the arduino and forward to a connected webclient
  * simple webclient page to connect to the server and send various commands
    * turn on/off pulses
    * rotate servo up/down
    * reset timeout
