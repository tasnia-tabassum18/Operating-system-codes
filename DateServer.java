/* 
    Modify the socket-based date server (Figure 3.27) in Chapter 3 so 
    that the server services each client request in a separate thread. 
*/
package DateServer;
import java.net.*;
import java.io.*;

public class DateServer {
    public static void main(String[] args) {

        try {
            ServerSocket sock = new ServerSocket(6013);

            /* now listen for connections */
            while (true) {
                Socket client = sock.accept();

                Runnable task = () -> {
                    try {
                        // Acquire client ostream and write Date to the socket
                        PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                        pout.println( 
                            (Thread.currentThread().getName()) + ": " + (new java.util.Date().toString()) 
                        );
                        client.close();
                    } catch (IOException e) {
                        System.out.println(e);
                    }
                };
                new Thread(task).start();
            }
        } catch (IOException ioe) {
            System.err.println(ioe);
        }
    }
}