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
    std::string address;
    std::string port;
    int maxNumConnections;

    public:
        Server(std::string address, std::string port, int maxNumConnections) : 
            address(address), port(port), maxNumConnections(maxNumConnections) { }
        virtual ~Server() = default;

        virtual void run()
        {
            zmq::context_t context = zmq::context_t(4, 16);
            zmq::socket_t serverSocket = zmq::socket_t(context, zmq::socket_type::rep);
            
            zmq::message_t message = zmq::message_t();
            zmq::proxy();

            int sockfd, client_sock;
            struct addrinfo hints;
            struct addrinfo* servinfo;
            struct addrinfo* p;
            struct sockaddr_storage clt_addr;
            socklen_t sin_size;
            char addr[INET6_ADDRSTRLEN];
            int yes = 1;
            int rc;
            int i = 2;

            std::vector<Worker> workers;
            std::vector<std::thread> threads;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            if ((rc = getaddrinfo(address.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
                std::cerr << "Could not get address information!" << std::endl;
                return;
            }

            for(p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    std::cerr << "Could not create socket!" << std::endl;
                    continue;
                }

                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                    std::cerr << "Could not configure socket!" << std::endl;
                    return;
                }

                if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                    close(sockfd);
                    std::cerr << "Could not bind socket!" << std::endl;
                    continue;
                }

                break;
            }

            freeaddrinfo(servinfo);

            if (p == NULL)  {
                std::cerr << "Server failed to bind!" << std::endl;
                return;
            }

            if (listen(sockfd, maxNumConnections) == -1) {
                std::cerr << "Server failed to listen!" << std::endl;
                return;
            }

            std::cout << "Server started and waiting for connections!" << std::endl;

            while(i > 0) {
                sin_size = sizeof(clt_addr);
                client_sock = accept(sockfd, (struct sockaddr *)&clt_addr, &sin_size);
                if (client_sock == -1) {
                    std::cerr << "Server failed to accept client!" << std::endl;
                    continue;
                }

                if (((struct sockaddr *) &clt_addr)->sa_family == AF_INET) {
                    inet_ntop(clt_addr.ss_family, &(((struct sockaddr_in*) &clt_addr)->sin_addr), addr, sizeof(addr));
                } else {
                    inet_ntop(clt_addr.ss_family, &(((struct sockaddr_in6*) &clt_addr)->sin6_addr), addr, sizeof(addr));
                }

                std::cout << "Server got connection from " << addr << "!" << std::endl;

                workers.push_back(Worker(client_sock));
                threads.push_back(std::thread(&Worker::run, &(workers.back())));
                i--;
            }

            for (int i = 0 ; i < threads.size(); i++) {
                threads[i].join();
            }
        }
};

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "USE: ./server <IP address> <port>" << std::endl;
        return 1;
    }

    std::string address(argv[1]);
    std::string port(argv[2]);

    Server server(address, port, 4);
    server.run();
    
    return 0;
}