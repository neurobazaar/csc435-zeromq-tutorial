#include "Worker.hpp"

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

void Worker::run(zmq::context_t& context) {
    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::rep);
    
    socket.connect("inproc://workers");

    while (true) {
        zmq::message_t request;

        socket.recv(request, zmq::recv_flags::none);
        
        std::string message = request.to_string();

        if (message.compare("quit") == 0) {
            break;
        }

        if (message.compare("addition") == 0) {
            std::string data{"2+2=4"};
            socket.send(zmq::buffer(data), zmq::send_flags::none);
            continue;
        }

        if (strcmp(buf, "multiplication") == 0) {
            memset(buf, 0, MAX_BUFFER_SIZE);
            strcpy(buf, "2x2=4");
            if (send(sock, buf, strlen(buf), 0) == -1) {
                std::cerr << "Error sending data!" << std::endl;
                break;
            }
            continue;
        }

        memset(buf, 0, MAX_BUFFER_SIZE);
        strcpy(buf, "???");
        if (send(sock, buf, strlen(buf), 0) == -1) {
            std::cerr << "Error sending data!" << std::endl;
            break;
        }
    }

    socket.close();

    delete buf;
}