#include <iostream>
#include "commands.cpp"
#include <string>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

int main(int argc, char const *argv[]) {
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
    int new_socket;
    int valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {};
    char* hello = "BASTIONBUDS";

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
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    //attach socket to port 8080
    int bind_sd = bind(server_fd,(struct sockaddr*)&address, sizeof(address));
    if(bind_sd < 0)
    {
        printf("Error binding socket to address\n");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for clients to connect...\n");
    listen(server_fd,5);
    
    new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
    
    valread = read(new_socket,buffer,1024);
    printf("%s/n",buffer);
    send(new_socket,hello,strlen(hello),0);
    printf("Hello message sent\n");
    return 0;


}
