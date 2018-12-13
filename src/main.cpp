#include <iostream>
#include "room_user.cpp"
#include "commands.cpp"
#include <string>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

int BUFFER_SIZE = 4096;

struct client_args{
    User *user;
    vector<Command*> commands;
};


vector<Room*> roomList;
//vector<Command*> commands;
pthread_mutex_t lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

char* receive(char* buffer,int socket)
{
    int a = sizeof(buffer);
    memset(buffer,0,a);//reset the buffer
    read(socket,buffer,BUFFER_SIZE);
    return buffer;
}


int transmit(int socket,char* message)
{
    //this method sends messages
    int output = send(socket,message,strlen(message),0);
    return output;
}



void *client_run(void *arg)
{
    cout << "1\n";
    cout<<"real beginning"<<endl;
    cout << "2\n";
    struct client_args *args = (client_args*)arg;
    cout << "3\n";
    User * user = args->user;
    Join *join = new Join(&roomList);
    Leave *leave = new Leave(&roomList);
    Rooms *rooms = new Rooms(&roomList);
    Who *who = new Who(&roomList);
    Help *help = new Help(&roomList);
    Quit *quit = new Quit(&roomList);
    Whisper *whisper = new Whisper(&roomList);
    ChessReset *chessReset = new ChessReset(&roomList);
    ChessMove *chessMove = new ChessMove(&roomList);
    ChessPrint *chessPrint = new ChessPrint(&roomList);
    UnreCom *unreCom = new UnreCom(&roomList);
    Message *message = new Message(&roomList);
    vector<Command*> commands{join, leave, rooms, who, help, quit, whisper, chessReset, chessMove, chessPrint, unreCom, message};


    int socket = user->getSocket();
    //cout<<"beginning of client_run"<<endl;
    while(1)
    {
        char* buf = new char[BUFFER_SIZE];
        //cout << "before receive" << endl;
        buf = receive(buf,socket);
        //cout << "after receive" << endl;
        //cout << commands.size() << endl;
        for(int i=0;i<commands.size();i++)
        {
            string strBuf = buf;
            if(commands[i]->matches(buf)) {
                pthread_mutex_lock(&lock);
                commands[i]->execute(buf,user);
                pthread_mutex_unlock(&lock);
                break;
            }
        }
    }
    return NULL;
}




int main() {
    Join *join = new Join(&roomList);
    Leave *leave = new Leave(&roomList);
    Rooms *rooms = new Rooms(&roomList);
    Who *who = new Who(&roomList);
    Help *help = new Help(&roomList);
    Quit *quit = new Quit(&roomList);
    Whisper *whisper = new Whisper(&roomList);
    ChessReset *chessReset = new ChessReset(&roomList);
    ChessMove *chessMove = new ChessMove(&roomList);
    ChessPrint *chessPrint = new ChessPrint(&roomList);
    UnreCom *unreCom = new UnreCom(&roomList);
    Message *message = new Message(&roomList);
    vector<Command*> commands{join, leave, rooms, who, help, quit, whisper, chessReset, chessMove, chessPrint, unreCom, message};

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    
    
    //socket file descriptor
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0)
    {
        printf("Error creating server socket");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsocketopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9003);
    
    //attach socket to port 8080
    int bind_sd = bind(server_fd,(struct sockaddr*)&address, sizeof(address));
    if(bind_sd < 0)
    {
        printf("Error binding socket to address\n");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for clients to connect...\n");
    listen(server_fd,5);
    cout << "Starting server. Press 'CTRL-C to close the server" << endl;
    while(true) {
        /* This is the main thread loop.
        *  This will loop indefinetly, until the user closes using "CTRL-C"
        *  This can be changed later to stop under certain conditions
        */
        
        int new_socket = accept(server_fd, (sockaddr *)&address,(socklen_t*)&addrlen);
        if(new_socket < 0)
        {
            printf("Error accepting client\n");
            exit(1);
        }
        User *user = new User(new_socket);
        pthread_t p1;
        int rc;
        struct client_args args = {user, commands};
        rc = pthread_create(&p1,NULL,client_run,&args);
    }
    return 0;

    
}




