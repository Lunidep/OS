#include <bits/stdc++.h>
#include "ZMQFunctions.h"
#include "unistd.h"

class CalculationNode {
    private:
        zmq:: context_t context;
    public:
        zmq:: socket_t left, right, parent;
        int left_port, right_port, parent_port;
        int id, left_id = -2, right_id = -2, parent_id;
        
        CalculationNode(int id, int parent_port, int parent_id): 
            id(id), 
            parent_port(parent_port), 
            parent_id(parent_id), 
            left(context, ZMQ_REQ),
            right(context, ZMQ_REQ), 
            parent(context, ZMQ_REP)
            {
            if (id != -1) {
                connect(parent, parent_port);
            }
        }
        string create (int child_id) {
            int port;
            bool isleft = false;
            if (left_id == -2) {
                left_port = bind(left, child_id);
                left_id = child_id;
                port = left_port;
                isleft = true;
            }
            else if (right_id == -2) {
                right_port = bind(right, child_id);
                right_id = child_id;
                port = right_port;
            }
            else {
                string fail = "Error: can not create the calculation node";
                return fail;
            }
            int fork_id = fork();
            if (fork_id == 0) {
                if (execl("./client", "client",  to_string(child_id).c_str(),  to_string(port).c_str(),  to_string(id).c_str(), (char*)NULL) == -1) {
                     cout << "Error: can not run the execl-command" <<  endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                 string child_pid;
                try {
                    if (isleft) {
                        left.setsockopt(ZMQ_SNDTIMEO, 3000);
                        send_message(left, "pid");
                        child_pid = receive_message(left);
                    }
                    else {
                        right.setsockopt(ZMQ_SNDTIMEO, 3000);
                        send_message(right, "pid");
                        child_pid = receive_message(right);
                    }
                    return "Ok: " + child_pid;
                }
                catch (int) {
                    string fail = "Error: can not connect to the child";
                    return fail;
                }
            }
        }
         string ping (int id) {
            string answer = "Ok: 0";
            if (this->id == id) {
                answer = "Ok: 1";
                return answer;
            }
            else if (left_id == id) {
                string message = "ping " +  to_string(id);
                send_message(left, message);
                try {
                    message = receive_message(left);
                    if (message == "Ok: 1") {
                        answer = message;
                    }
                }
                catch(int){}
            }
            else if (right_id == id) {
                string message = "ping " +  to_string(id);
                send_message(right, message);
                try {
                    message = receive_message(right);
                    if (message == "Ok: 1") {
                        answer = message;
                    }
                }
                catch(int){}
            }
            return answer;
        }

        string sendstring (string string, int id) {
            std::string answer = "Error: Parent not found";
            if (left_id == -2 && right_id == -2) {
                return answer;
            }
            else if (left_id == id) {
                if (ping(left_id) == "Ok: 1") {
                    send_message(left, string);
                    try{
                        answer = receive_message(left);
                    }
                    catch(int){}
                }
            }
            else if (right_id == id) {
                if (ping(right_id) == "Ok: 1") {
                    send_message(right, string);
                    try {
                        answer = receive_message(right);
                    }
                    catch(int){}
                }
            }
            else {
                if (ping(left_id) == "Ok: 1") {
                    std::string message = "send " +  to_string(id) + " " + string;
                    send_message(left, message);
                    try {
                        message = receive_message(left);
                    }
                    catch(int) {
                        message = "Error: Parent not found";
                    }
                    if (message != "Error: Parent not found") {
                        answer = message;
                    }
                }
                if (ping(right_id) == "Ok: 1") {
                    std::string message = "send " +  to_string(id) + " " + string;
                    send_message(right, message);
                    try {
                        message = receive_message(right);
                    }
                    catch(int) {
                        message = "Error: Parent not found";
                    }
                    if (message != "Error: Parent not found") {
                        answer = message;
                    }
                }
            }
            return answer;
        }
        
        string exec ( string string) {
            istringstream string_thread(string);
            int result = 0;
            int amount, number;
            string_thread >> amount;
            for (int i = 0; i < amount; ++i) {
                string_thread >> number;
                result += number;
            }
            std::string answer = "Ok: " +  to_string(id) + ": " +  to_string(result);
            return answer;
        }

        string kill () { 
            if (left_id != -2){
                if (ping(left_id) == "Ok: 1") {
                    std::string message = "kill";
                    send_message(left, message);
                    try {
                        message = receive_message(left);
                    }
                    catch(int){}
                    unbind(left, left_port);
                    left.close();
                }
            }
            if (right_id != -2) {
                if (ping(right_id) == "Ok: 1") {
                    std::string message = "kill";
                    send_message(right, message);
                    try {
                        message = receive_message(right);
                    }
                    catch (int){}
                    unbind(right, right_port);
                    right.close();
                }
            }
            return  to_string(parent_id);
        }
        ~CalculationNode() {}
};