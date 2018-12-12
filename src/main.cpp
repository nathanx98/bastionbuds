#include <iostream>
#include "commands.cpp"
#include <string>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;


//read message from client
string receive(char* buffer,int socket)
{
    memset(&buffer,0,sizeof(buffer));//reset the buffer
    read(socket,buffer,1024);
    string output(buffer);
    return output;
}



int main() {
    /*vector<Command*> commands{&join, &leave};

    cout << "Hello, World!" << endl;
    cout << "join matches: " << commands.at(0)->matches("lol") << endl;
    cout << "join isValid: " << commands.at(0)->isValid("lol") << endl;
    cout << "join execute: " << commands.at(0)->execute("lol", "no") << endl;

    cout << "leave matches: " << commands.at(1)->matches("lol") << endl;
    cout << "leave isValid: " << commands.at(1)->isValid("lol") << endl;
    cout << "leave execute: " << commands.at(1)->execute("lol", "no") << endl;
     */
    
    int server_fd;
    int valread;
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
    int new_socket = accept(server_fd, (sockaddr *)&address,(socklen_t*)&addrlen);
    if(new_socket < 0)
    {
        printf("Error accepting client\n");
        exit(1);
    }
    printf("connection successful\n");
    
    //valread = read(new_socket,buffer, 1024);
    string ouput = receive(buffer,new_socket);
    printf("%s\n",buffer );
    if(strcmp(buffer,"exit"))
    {
        printf("exit\n");
        exit(0);
    }
    send(new_socket,message,strlen(message),0);
    printf("Hello message sent\n");
    return 0;
}



//
//int send()
//{
//    //this method sends messages
//
//    return 0;
//}
