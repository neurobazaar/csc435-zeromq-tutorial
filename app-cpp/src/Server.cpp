#include "Server.hpp"

#include <iostream>
#include <vector>
#include <thread>
#include <memory>

#include "Worker.hpp"

void Server::run()
{
    std::vector<std::shared_ptr<Worker>> workers;
    std::vector<std::thread> threads;

    // Create ZMQ router and dealer sockets
    zmq::socket_t routerSocket = zmq::socket_t(context, zmq::socket_type::router);
    zmq::socket_t dealerSocket = zmq::socket_t(context, zmq::socket_type::dealer);

    // Bind the router socket to the server listening address and port
    // Bind the dealer socket to internal communcation channel
    routerSocket.bind("tcp://" + address + ":" + port);
    dealerSocket.bind("inproc://workers");

    // Create worker threads that connect to the dealer
    for (auto i = 0; i < numWorkers; i++) {
        std::shared_ptr<Worker> worker = std::make_shared<Worker>(std::ref(*this));
        workers.push_back(worker);
        threads.push_back(std::thread(&Worker::run, worker, std::ref(context)));
    }

    // Create the ZMQ queue that forwards messages between the router and the dealer
    try {
        zmq::proxy(routerSocket, dealerSocket);
    } catch(zmq::error_t& error) {
        std::cout << "Terminating the server!" << std::endl;
    }

    for (auto i = 0; i < numWorkers; i++) {
        threads[i].join();
    }

    routerSocket.close();
    dealerSocket.close();
    context.close();
}

void Server::workerTerminated()
{
    numTerminatedWorkers++;

    // Stop after two workers terminated
    if (numTerminatedWorkers >= 2) {
        context.shutdown();
    }
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "USE: ./server <IP address> <port> <number of worker threads>" << std::endl;
        return 1;
    }

    std::string address(argv[1]);
    std::string port(argv[2]);
    int numWorkers(std::atoi(argv[3]));

    Server server(address, port, numWorkers);
    server.run();
    
    return 0;
}