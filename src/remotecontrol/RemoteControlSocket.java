package remotecontrol;

import org.eclipse.jetty.websocket.api.Session;
import org.eclipse.jetty.websocket.api.WebSocketAdapter;

import java.io.IOException;

public class RemoteControlSocket extends WebSocketAdapter implements ArduinoListener {
    public static String LOGIN_MSG = "login";
    public static String LOGIN_STATUS_MSG = "loggedin";
    public static String PASSWORD = "10993";

    public ArduinoCommunicator arduino = new ArduinoCommunicator();

    boolean loggedIn = false;

    /**
     *
     * @param sess
     */
    @Override
    public void onWebSocketConnect(Session sess) {
        super.onWebSocketConnect(sess);
        System.out.println("connection from remote: " + sess.getRemoteAddress().toString());
        System.out.println("Socket Connected: " + sess);
    }

    /**
     * messages initiated from the websocket client
     *
     * @param message
     */
    @Override
    public void onWebSocketText(String message) {
        super.onWebSocketText(message);
        if (loggedIn) {
            arduino.sendLine(message);
        } else {
            if (message.equals(LOGIN_MSG + ":" + PASSWORD)) {
                if (arduino.initialize()) {
                    loggedIn = true;
                    arduino.setListener(this);
                    try {
                        this.getRemote().sendString(LOGIN_STATUS_MSG + ":true");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    arduino.sendLine(ArduinoCommunicator.KEEP_GOING_CMD);
                } else {
                    System.out.println("unable to connect to arduino or another client is already connected");
                    this.getSession().close();
                }
            } else {
                System.out.println("ignoring message:" + message);
            }
        }
    }

    /**
     *
     * @param statusCode
     * @param reason
     */
    @Override
    public void onWebSocketClose(int statusCode, String reason) {
        super.onWebSocketClose(statusCode, reason);
        System.out.println("Socket Closed: [" + statusCode + "] " + reason);
        arduino.close();
    }

    /**
     *
     * @param cause
     */
    @Override
    public void onWebSocketError(Throwable cause) {
        super.onWebSocketError(cause);
        cause.printStackTrace(System.err);
    }

    /**
     * messages initiated from the arduiono
     *
     * @param inputLine
     */
    @Override
    public void receiveFromArduino(String inputLine) {
        try {

            if (loggedIn) {
                // forward the arduino message to the connected websocket client
                this.getRemote().sendString(inputLine);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}