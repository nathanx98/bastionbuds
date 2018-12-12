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

using namespace std;


bool clientConnecting = false;
vector<Room*> roomList;

char* receive(char* buffer,int socket)
{
    memset(buffer,0,sizeof(buffer));//reset the buffer
    read(socket,buffer,1024);
    return buffer;
}


int transmit(int socket,char* message)
{
    //this method sends messages
    int output = send(socket,message,strlen(message),0);
    return output;
}



int main() {
    Join join = Join(&roomList);
    Leave leave = Leave(&roomList);
    Rooms rooms = Rooms(&roomList);
    Who who = Who(&roomList);
    Help help = Help(&roomList);
    Quit quit = Quit(&roomList);
    Whisper whisper = Whisper(&roomList);
    UnreCom unreCom = UnreCom(&roomList);
    Message message = Message(&roomList);

    vector<Command*> commands{&join, &leave};
    User gerald = User(12);
    Room room = Room("the_wOw_room");

    cout << "room name: " << room.getRoomName() << endl;
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.addUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.removeUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.removeUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;

    string word = "woah!";
    cout << word.substr(0, word.find(" ")) << endl;

    cout << "Hello, World!" << endl;
    cout << "join matches: " << commands.at(0)->matches("lol") << endl;
    cout << "join isValid: " << commands.at(0)->isValid("lol", &gerald) << endl;
    cout << "join execute: " << commands.at(0)->execute("lol", &gerald) << endl;

    cout << "leave matches: " << commands.at(1)->matches("lol") << endl;
    cout << "leave isValid: " << commands.at(1)->isValid("lol", &gerald) << endl;
    cout << "leave execute: " << commands.at(1)->execute("lol", &gerald) << endl;


    
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {};
    char* message = "BASTIONBUDS";
    
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
    address.sin_port = htons(9002);
    
    //attach socket to port 8080
    int bind_sd = bind(server_fd,(struct sockaddr*)&address, sizeof(address));
    if(bind_sd < 0)
    {
        printf("Error binding socket to address\n");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for clients to connect...\n");
    listen(server_fd,5);
    printf("the line before accept\n");
    new_socket = accept(server_fd, (sockaddr *)&address,(socklen_t*)&addrlen);
    if(new_socket < 0)
    {
        printf("Error accepting client\n");
        exit(1);
    }
    printf("connection successful\n");
    
    char* output = receive(buffer,new_socket);
    printf("%s\n",output);
    if(strcmp(output,"exit")==0)
    {
        printf("client exited\n");
        exit(0);
    }
    transmit(new_socket,message);
    printf("Hello message sent\n");



    clientConnecting = false;

    cout << "Starting server. Press 'CTRL-C to close the server" << endl;
    while(true) {
        /* This is the main thread loop.
        *  This will loop indefinetly, until the user closes using "CTRL-C"
        *  This can be changed later to stop under certain conditions
        */

        if(clientConnecting) {
            //TODO: Lock mutex
            cout << "Client connection to server..." << endl;

            //TODO: Add client to global list of users

            //TODO: Spawn Listen thread
            bool connectionSuccessful = create_thread();

            if (connectionSuccessful)   {cout << "Client successfully connected to sever!" << endl;}
            else                        {cout << "Client could not connect to server!" << endl;}
            //TODO: Unlock Mutex

        }


    return 0;
}



