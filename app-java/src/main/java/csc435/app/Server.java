package csc435.app;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;

public class Server {
    
    private String address;
    private Integer port;
    private Integer maxNumConnections;

    public Server(String address, int port, int maxNumConnections) {
        this.address = address;
        this.port = port;
        this.maxNumConnections = maxNumConnections;
    }

    public void run() {
        try {
            ArrayList<Thread> threads = new ArrayList<Thread>();
            ServerSocket serverSocket = new ServerSocket(port, maxNumConnections, InetAddress.getByName(address));
            int i = 2;

            System.out.println("Server started and waiting for connections!");

            while (i > 0) {
                Socket clientSocket = serverSocket.accept();

                System.out.println("Server got connection from " + clientSocket.getInetAddress());

                Thread td = new Thread(new Worker(clientSocket));
                td.start();
                threads.add(td);
                i--;
            }

            serverSocket.close();

            for (Thread td : threads) {
                td.join();
            }
        } catch (UnknownHostException e) {
            System.err.println("Could not compute IP address!");
            e.printStackTrace();
        } catch (IOException e) {
            System.err.println("Socket error!");
            e.printStackTrace();
        } catch (InterruptedException e) {
            System.err.println("Thread join error!");
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("USE: java Server <IP address> <port>");
            System.exit(1);
        }

        Server server = new Server(args[0], Integer.parseInt(args[1]), 4);
        server.run();
    }
}
