#include <iostream>
#include <string>
#include <vector>
#include <thread>

extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
}

#include <zmq.hpp>
#include "Worker.hpp"

class Server
{
    int numWorkers;
    std::string address;
    std::string port;
    int maxNumConnections;

    public:
        Server(int numWorkers, std::string address, std::string port, int maxNumConnections) : 
            numWorkers(numWorkers), address(address), port(port), maxNumConnections(maxNumConnections) { }
        virtual ~Server() = default;

        virtual void run()
        {
            std::vector<Worker> workers;
            std::vector<std::thread> threads;

            zmq::context_t context = zmq::context_t(4, maxNumConnections);

            zmq::socket_t routerSocket = zmq::socket_t(context, zmq::socket_type::router);
            zmq::socket_t dealerSocket = zmq::socket_t(context, zmq::socket_type::dealer);

            routerSocket.bind("tpc://" + address + ":" + port);
            dealerSocket.bind("inproc://workers");

            for (auto i = 0; i < numWorkers; i++) {
                workers.push_back(Worker());
                threads.push_back(std::thread(&Worker::run, &(workers.back()), std::ref(context)));
            }

            zmq::proxy(routerSocket, dealerSocket, NULL);

            routerSocket.close();
            dealerSocket.close();
            context.close();
        }
};

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "USE: ./server <number of worker threads> <IP address> <port>" << std::endl;
        return 1;
    }

    std::string numWorkers(argv[1]);
    std::string address(argv[2]);
    std::string port(argv[3]);

    Server server(numWorkers, address, port, 16);
    server.run();
    
    return 0;
}