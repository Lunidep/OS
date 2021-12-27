#include "funcs.h"

int in(vector<string> logins, string str) {
    for (int i = 0; i < logins.size(); ++i) {
        if (logins[i] == str)
            return i;
    }
    return -1;
}

int main(){
    vector<string> logins;
    vector<string> chats;
    
    cout << "Enter number of users\n";
    int n; cin >> n;
    cout << "\nEnter all user's logins or chat's names.\n";

    while (n>0) {
        string login;
        cin >> login;
        if (login == "") { break; }
        if (login.find("chat") == 0){
            if (in(chats, login) == -1){
                chats.push_back(login);
            }
            else{
                cout << "Chat already exists!\n\n";
            }
        }
        else{
            if (in(logins, login) == -1){
                logins.push_back(login);
            }
            else{
                cout << "Login already exists!\n\n";
            }
        }
        n--;
    }
    cout << "Instant messaging system is started!\n";

    //создание и открытие входного FIFO
    if (mkfifo("input", 0777) == -1)  {
        cout << "MAIN INPUT FIFO WAS NOT CREATED";
        exit(1);
    }
    int fd_recv = open("input", O_RDONLY);
    if (fd_recv == -1) {
        cout << "INPUT FIFO WAS NOT OPENED";
        exit(1);
    }

    //создание и открытие выходных FIFO для всех логинов
    for (int i = 0; i < logins.size(); ++i)    {
        if (mkfifo(logins[i].c_str(), 0777) == -1) {
            cout << "FIFO WAS NOT CREATED";
            exit(1);
        }
    }
    for (int i = 0; i < chats.size(); ++i)    {
        if (mkfifo(chats[i].c_str(), 0777) == -1) {
            cout << "FIFO WAS NOT CREATED";
            exit(1);
        }
    }

    int fd_l[logins.size()];
    for (int i = 0; i < logins.size(); ++i) {
        fd_l[i] = open(logins[i].c_str(), O_WRONLY);
        if (fd_l[i] == -1) {
            cout << "FIFO login WAS NOT OPENED";
            exit(1);
        }
    }

    int fd_ch[chats.size()];
    for (int i = 0; i < chats.size(); ++i) {
        fd_ch[i] = open(chats[i].c_str(), O_WRONLY);
        if (fd_ch[i] == -1) {
            cout << "FIFO chat WAS NOT OPENED";
            exit(1);
        }
    }

    //обработка сообщений, полученных от клиентов
    while (true) {
        string message;
        message = s_recieve(fd_recv); //читаем из input файла
        //cout << "message  " << message << endl;

        string f_sender = find_sender(message);
        string f_recipient = find_recipient(message);
        string f_message_info = find_message_info(message);

        int fd_sender = in(logins, f_sender);

        int fd_recipient;
        if (in(logins, f_recipient) != -1){
            fd_recipient = in(logins, f_recipient);
            s_send(fd_l[fd_recipient], f_message_info);
        } 
        else if (in(chats, f_recipient) != -1){
            fd_recipient = in(chats, f_recipient);
            s_send(fd_ch[fd_recipient], f_message_info);
        } 
        else{
            s_send(fd_l[fd_sender], "Login does not exists!");
        }

        // cout << "---------------------------------------------\n";
        // cout << "f_sender  " << f_sender << endl;
        // cout << "f_recipient  " << f_recipient << endl;
        // cout << "f_message_info  " << f_message_info << endl << endl;
        // cout << "fd_recipient  " << fd_recipient << endl;
        // cout << "fd_sender  " << fd_sender << endl;
        // cout << "---------------------------------------------\n\n";
    }
}
