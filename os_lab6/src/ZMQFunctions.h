#ifndef ZMQ_H
#define ZMQ_H
#include <bits/stdc++.h>
#include <zmq.hpp>
const int MAIN_PORT = 4040;

using namespace std;

void send_message(zmq::socket_t &socket, const string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message);
}

string receive_message(zmq::socket_t &socket) {
    zmq::message_t message;
    int chars_read;
    try {
        chars_read = (int)socket.recv(&message);
    }
    catch (...) {
        chars_read = 0;
    }
    if (chars_read == 0) {
        throw -1;
    }
    string received_msg(static_cast<char*>(message.data()), message.size());
    return received_msg;
}

void connect(zmq::socket_t &socket, int port) {
    string address = "tcp://127.0.0.1:" + to_string(port);
    socket.connect(address);
}

void disconnect(zmq::socket_t &socket, int port) {
    string address = "tcp://127.0.0.1:" + to_string(port);
    socket.disconnect(address);
}

int bind(zmq::socket_t &socket, int id) {
    int port = MAIN_PORT + id;
    string address = "tcp://127.0.0.1:" + to_string(port);
    while(1){
        try{
            socket.bind(address);
            break;
        }
        catch(...){
            port++;
        }
    }
    return port;
}

void unbind(zmq::socket_t &socket, int port) {
    string address = "tcp://127.0.0.1:" + to_string(port);
    socket.unbind(address);
}

#endif