#ifndef WORKER_H
#define WORKER_H

#include <iostream>

#include <zmq.hpp>

#include "Server.hpp"

class Worker
{
    Server& server;

    public:
        Worker(Server& server) : server(server) {  }
        virtual ~Worker() = default;

        virtual void run(zmq::context_t& context);
};

#endif