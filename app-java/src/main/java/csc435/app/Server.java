package csc435.app;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;

import org.zeromq.SocketType;
import org.zeromq.ZContext;
import org.zeromq.ZMQ;

public class Server {
    
    private String address;
    private String port;
    private Integer numWorkers;

    private ZContext context;

    public Server(String address, String port, int numWorkers) {
        this.address = address;
        this.port = port;
        this.numWorkers = numWorkers;
    }

    public void run() {
        // ZMQ context initialized with 4 IO threads
        context = new ZContext(4);
        
        // Create ZMQ router and dealer sockets
        ZMQ.Socket routerSocket = context.createSocket(SocketType.ROUTER);
        ZMQ.Socket dealerSocket = context.createSocket(SocketType.DEALER);

        // Bind the router socket to the server listening address and port
        // Bind the dealer socket to internal communcation channel
        routerSocket.bind("tcp://" + address + ":" + port);
        dealerSocket.bind("inproc://workers");

        for (int i = 0; i < numWorkers; i++) {
            Worker worker = new Worker(this, context);
        }

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
        if (args.length != 3) {
            System.err.println("USE: java Server <IP address> <port> <number of workers");
            System.exit(1);
        }

        Server server = new Server(args[0], args[1], Integer.parseInt(args[2]));
        server.run();
    }
}
