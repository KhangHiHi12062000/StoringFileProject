import java.net.*;
import java.io.*;

public class Client {
    // initialize socket and input output streams
    private Socket socket;
    private DataInputStream  input;
    private DataOutputStream out;

    // constructor to put ip address and port
    public Client(String address, int port)
    {
        // establish a connection
        try
        {
            socket = new Socket(address, port);
            System.out.println("Connected");

            // takes input from terminal
            input  = new DataInputStream(System.in);

            // sends output to the socket
            out    = new DataOutputStream(socket.getOutputStream());

            out.writeChar('0');
//            out.writeInt(4222);
//            out.writeInt(10);
            StringBuilder sb = new StringBuilder();
            char c = input.readChar();
            while (c != '\0') {
                sb.append(c);
                c = input.readChar();
            }

            System.out.println(sb);
            // close connection
            input.close();
            out.close();
            socket.close();
        } catch(IOException u)
        {
            System.out.println(u);
        }

    }

    public static void main(String args[])
    {
        Client client = new Client("localhost", 8888);
    }
}
