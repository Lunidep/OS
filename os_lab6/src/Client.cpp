#include <bits/stdc++.h>  
#include "CalculationNode.h"    
#include "ZMQFunctions.h"
#include "BalancedTree.h"  


int main(int argc, char *argv[]) {

    CalculationNode node(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    while(true) {
        string message;
        string command;
        message = receive_message(node.parent);
        istringstream request(message);
        request >> command;
        if (command == "pid") {
            string answer =  to_string(getpid());
            send_message(node.parent, answer);
        }
        else if (command == "ping") {
            int child;
            request >> child;
            string answer = node.ping(child);
            send_message(node.parent, answer);
        }
        else if (command == "create") {
            int child;
            request >> child;
            string answer = node.create(child);
            send_message(node.parent, answer);
        }
        else if (command == "exec") {
            string str;
            getline(request, str);
            string answer = node.exec(str);
            send_message(node.parent, answer);
        }
        else if (command == "kill") {
            string answer = node.kill();
            send_message(node.parent, answer);
            disconnect(node.parent, node.parent_port);
            node.parent.close();
            break;
        }
    }
    return 0;
}