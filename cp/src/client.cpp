#include "funcs.h"
#include <thread>

//функция приёма сообщений (для потока)
void func(int fd_recv, string login){
    while (true){
        string reply = c_recieve(fd_recv);
        if (login.find("chat") == -1){
            //cout << reply << "\n";
            cout.flush();
            cout << login << ">";
            
        }cout.flush();
    }
}


int main(){
    //подключение к входному FIFO сервера
    int fd_send = open("input", O_WRONLY);
    if (fd_send == -1)  {
        cout << "ERROR: MAIN FIFO WAS NOT OPENED\n";
        exit(1);
    }

    cout << "Insert login or chat name: ";
    string login;

    //подключение к персональному именованному пайпу
    int fd_recv = -1;
    while (fd_recv == -1)  {
        cin >> login;
        fd_recv = open(login.c_str(), O_RDONLY);
        if (fd_recv == -1) {
            cout << "Wrong login!\nInsert your login: ";
        }
    };

    
    string adressee, message;
    cout << "You have successfully signed!\n";

    //запуск потока принятия сообщений от сервера
    thread thr_recieve(func, fd_recv, login);

    //запуск цикла отправки сообщений на сервер
    if (login.find("chat") == -1){//user
        cout << "USAGE: <recipient's login> <your message>\n\tquit - completion of work\n"; 
        
        while (true) {
            cout << login <<"> ";
            cin >> adressee;

            if (adressee == "quit")
                break;
            getline(cin, message);
            c_send(fd_send, login, adressee, message);
        }
    }
    else{//chat
        cout << "\tquit - completion of work\n\n"; 

        while (true) {
            cin >> adressee;
            if (adressee == "quit")
                break;
        }
    }

    thr_recieve.detach();
}