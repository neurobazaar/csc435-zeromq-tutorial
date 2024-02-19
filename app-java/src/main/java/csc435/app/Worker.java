package csc435.app;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Worker implements Runnable {
    private Socket socket;

    public Worker(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try{
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            String inputLine;

            while ((inputLine = in.readLine()) != null) {
                if (inputLine.compareTo("quit") == 0) {
                    break;
                }

                if (inputLine.compareTo("addition") == 0) {
                    String outputLine = "2+2=4";
                    out.println(outputLine);
                    continue;
                }

                if (inputLine.compareTo("multiplication") == 0) {
                    String outputLine = "2x2=4";
                    out.println(outputLine);
                    continue;
                }
                
                String outputLine = "???";
                out.println(outputLine);
            }
        } catch (IOException e) {
            System.out.println("Socket IO error!");
            e.printStackTrace();
        }
    }
}
