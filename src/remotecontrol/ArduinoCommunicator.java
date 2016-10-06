package remotecontrol;

import gnu.io.*;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Enumeration;

public class ArduinoCommunicator implements SerialPortEventListener {
    public static final String UNLOCK_CMD = "U";
    public static final String ON_CMD = "O";
    public static final String OFF_CMD = "F";
    public static final String UP_CMD = "I";
    public static final String DOWN_CMD = "D";
    public static final String MINIMUM_CMD = "M";
    public static final String KEEP_GOING_CMD = "K";

    SerialPort serialPort = null;
    ArduinoListener listener;
    boolean initialized = false;
    private static final String PORT_NAMES[] = { "/dev/ttyACM" };

    private String appName;
    private BufferedReader input;
    private OutputStream output;

    private static final int TIME_OUT = 1000; // Port open timeout
    private static final int DATA_RATE = 9600; // Arduino serial port

    public ArduinoCommunicator() {
        appName = getClass().getName();
    }

    public void setListener(ArduinoListener listener) {
        this.listener = listener;
    }

    /**
     * find a usb port and connect
     *
     * @throws PortInUseException
     */
    private void connectOnAnyPort() throws PortInUseException {
        CommPortIdentifier portId = null;
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

        System.out.println( "Trying:");
        while (portId == null && portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
            System.out.println( "   port" + currPortId.getName() );
            for (String portName : PORT_NAMES) {
                if ( currPortId.getName().equals(portName)
                        || currPortId.getName().startsWith(portName)) {

                    // Try to connect to the Arduino on this port
                    //
                    // Open serial port
                    serialPort = (SerialPort)currPortId.open(appName, TIME_OUT);
                    portId = currPortId;
                    System.out.println( "Connected on port" + currPortId.getName() );
                    break;
                }
            }
        }

        if (portId == null || serialPort == null) {
            System.out.println("Oops... Could not connect to Arduino");
            initialized = false;
        }
    }

    public boolean initialize() {
        try {
            connectOnAnyPort();

            if (serialPort != null) {
                serialPort.setSerialPortParams(DATA_RATE,
                        SerialPort.DATABITS_8,
                        SerialPort.STOPBITS_1,
                        SerialPort.PARITY_NONE);
                serialPort.addEventListener(this);
                serialPort.notifyOnDataAvailable(true);

                // Give Arduino time to respond
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException ie) {
                }

                initialized = true;
                return true;
            }
        }
        catch ( Exception e ) {
            e.printStackTrace();
        }
        return false;
    }

    /**
     * transmit data over serial port
     * @param data
     */
    private void sendData(String data) {
        try {
            output = serialPort.getOutputStream();
            output.write( data.getBytes() );
        }
        catch (Exception e) {
            System.err.println(e.toString());
            System.exit(0);
        }
    }

    /**
     * send data with newline added
     * @param data
     */
    public void sendLine(String data) {
        if (initialized) {
            sendData(data + "\n");
        }
    }

    /**
     * called when you stop using the port
     */
    public synchronized void close() {
        if ( serialPort != null ) {
            serialPort.removeEventListener();
            serialPort.close();

            System.out.println("serial port closed");
        }
    }

    //
    // Handle serial port event
    //
    public synchronized void serialEvent(SerialPortEvent oEvent) {
        try {
            switch (oEvent.getEventType() ) {
            case SerialPortEvent.DATA_AVAILABLE:
                // initialize buffer just once per stream, then reuse
                if ( input == null ) {
                    input = new BufferedReader(
                            new InputStreamReader(
                                    serialPort.getInputStream()));
                }
                String inputLine;
                while (input.ready()) {
                    // read one line per message and forward to the listener
                    inputLine = input.readLine();
                    System.out.println("received:" + inputLine);
                    if (listener != null) {
                        listener.receiveFromArduino(inputLine);
                    }
                }
                break;

            default:
                break;
            }
        }
        catch (Exception e) {
            System.err.println(e.toString());
            e.printStackTrace();
        }
    }
}