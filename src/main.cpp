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

//read message from client, returns the message read
char* receive(char* buffer,int socket)
{
    int a = sizeof(buffer);
    memset(buffer,0,a);//reset the buffer
    read(socket,buffer,BUFFER_SIZE);
    return buffer;
}

//send message to client with socket
//returns #of bytes sent, on error returns -1
int transmit(int socket,char* message)
{
    //this method sends messages
    int output = send(socket,message,strlen(message),0);
    return output;
}


//This function becomes a thread for each individual client
//pass this function as a param to pthread_create
void *client_run(void *arg)
{
    struct client_args *args = (client_args*)arg;
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
    int i = 0;
    while(1)
    {
        char* buf = new char[BUFFER_SIZE];
        buf = receive(buf,socket);
        for(i=0;i<commands.size();i++)
        {
            string strBuf = buf;
            if(commands[i]->matches(buf)) {
                pthread_mutex_lock(&lock);
                commands[i]->execute(buf,user);
                pthread_mutex_unlock(&lock);
                break;
            }
        }
        if(i == 5) break;   //this deals with /QUIT, which is the 5th commmand in vector
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
    
    /*  Creating the socket and
     
     */
    
    
    
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
    /*  This is supposed to help with address and ports reusing
     *  and prevents errors like address already in use,
        and it works like magic.
     */
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsocketopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9003);     //the port and address
    
    //attach socket to port
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
        struct client_args args = {user, commands};
        //creating new threads here. each client has it's corresponding thread
        pthread_create(&p1,NULL,client_run,&args);
    }
    return 0;

    
}




