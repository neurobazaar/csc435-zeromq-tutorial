#ifndef WORKER_H
#define WORKER_H

#include <iostream>

#include <zmq.hpp>

class Worker
{
    public:
        Worker() { }
        virtual ~Worker() = default;

        virtual void run(zmq::context_t& context);
};

#endif