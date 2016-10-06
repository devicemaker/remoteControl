package remotecontrol;

/**
 * listen to events coming back from the arduino
 */
public interface ArduinoListener {
    void receiveFromArduino(String inputLine);
}
