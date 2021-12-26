#include <bits/stdc++.h>  
#include "CalculationNode.h"
#include "ZMQFunctions.h" 
#include "BalancedTree.h"

void menu(){
    cout << "Avaliable commands:\n";
    cout << "1. create <id>\n";
    cout << "2. exec <id> <n> <n1 n2... n>\n";
    cout << "3. ping <id>\n";
    cout << "4. kill <id>\n";
}


int main() {
    string command;
    CalculationNode node(-1, -1, -1);
    string answer;
    BalancedTree tree;

    menu();

    while (( cout << "> ") && ( cin >> command)) {

        if (command == "create") {
            int child; cin >> child;
            
            if (tree.Exist(child)) {
                 cout << "Error: Already exists\n";
            }
            else {
                while (true) {
                    int idParent = tree.FindID();
                    if (idParent == node.id) {
                        answer = node.create(child);
                        tree.AddInTree(child, idParent);
                        break;
                    }
                    else {
                        string message = "create " +  to_string(child);
                        answer = node.sendstring(message, idParent);
                        if (answer == "Error: Parent not found") {
                            tree.AvailabilityCheck(idParent);
                        }
                        else {
                            tree.AddInTree(child, idParent);
                            break;
                        }
                    }
                }
                cout << answer << endl;
            }
        }

        else if (command == "exec") {

            if (!tree.Exist(child)) {
                 cout << "Error: Node does not exist!\n";
            }

            string str;
            int child; cin >> child;
            getline(cin, str);

            if (!tree.Exist(child)) {
                cout << "Error: Parent is not existed\n";
            }
            else {
                string message = "exec " + str;
                answer = node.sendstring(message, child);
                cout << answer <<  endl;
            }
        } 

        else if (command == "ping") {
            int child; cin >> child;

            if (!tree.Exist(child)) {
                cout << "Ok: 0\n";
            }
            else if (node.left_id == child || node.right_id == child) {
                answer = node.ping(child);
                cout << answer <<  endl;
            }
            else {
                string message = "ping " +  to_string(child);
                answer = node.sendstring(message, child);
                cout << answer <<  endl;
            } 
        }

        else if (command == "kill") {
            int child; cin >> child;

            string message = "kill";
            if (!tree.Exist(child)) {
                cout << "Error: Parent is not existed\n";
            }
            else {
                answer = node.sendstring(message, child);
                if (answer != "Error: Parent not found") {
                    tree.RemoveFromRoot(child);
                    if (child == node.left_id){
                        node.left_id = -2;
                        unbind(node.left, node.left_port);
                        answer = "Ok";
                    }
                    else if (child == node.right_id) {
                        node.right_id = -2;
                        unbind(node.right, node.right_port);
                        answer = "Ok";
                    }
                    else {
                        message = "clear " +  to_string(child);
                        answer = node.sendstring(message,  stoi(answer));
                    }
                    cout << answer <<  endl;
                }
            }
        }
        else {
            cout << "Please enter correct command!\n\n";
            menu();
        }
    }
    node.kill();
    return 0;
}