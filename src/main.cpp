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
    read(socket,buffer,1024);
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
    cout<<"beginning of client_run"<<endl;
    while(1)
    {
        char* buf = new char[1024];
        cout << "before receive" << endl;
        buf = receive(buf,socket);
        cout << "after receive" << endl;
        cout << commands.size() << endl;
        for(int i=0;i<commands.size();i++)
        {
            cout<<"for loop"<< commands[i] << "  " << buf << endl;
            string strBuf = buf;
            if(commands[i]->matches(buf)) {
                cout<<"for loop1"<<endl;
                pthread_mutex_lock(&lock);
                cout<<"for loop2"<<endl;
                commands[i]->execute(buf,user);
                cout<<"for loop3"<<endl;
                pthread_mutex_unlock(&lock);
                cout<<"for loop4"<<endl;
                break;
            }
            cout<<"for loop5"<<endl;
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



    User *gerald = new User(12);
    Room *room = new Room("the_wOw_room");
    roomList.push_back(room);
    room->addUser(gerald);
    gerald->setRoom(room);

    string buf = "/CHESSMOVE 22 1 4 1";
    for(int i=0;i<commands.size();i++)
        {
            if(commands[i]->matches(buf)) {
                pthread_mutex_lock(&lock);
                commands[i]->execute(buf,gerald);
                pthread_mutex_unlock(&lock);
                break;
            }
        }
    cout << room->chessBoardString() << endl;
    //Room room = Room("the_wOw_room");
    /*
    ChessBoard chess = ChessBoard();

    cout << chess.boardString() << endl;
    chess.movePiece(1,1, 2,1);
    chess.movePiece(111,1, 2,1);
    cout << chess.boardString() << endl;

    cout << "room name: " << room.getRoomName() << endl;
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.addUser(&gerald);
    gerald.setRoom(&room);
    gerald.setNickname("gerald");
    cout << "room size: " << room.listOfUsers.size() << endl;
    //room.removeUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;
    //room.removeUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;
    roomList.push_back(&room);
    */
    /*
    cout << "Hello, World!" << endl;
    cout << "join matches: " << commands.at(0)->matches("lol") << endl;
    cout << "join isValid: " << commands.at(0)->isValid("/JOIN 0v0b 0 0", gerald) << endl;
    //cout << "join execute: " << commands.at(0)->execute("lol", &gerald) << endl;

    cout << "leave matches: " << commands.at(1)->matches("lol") << endl;
    cout << "leave isValid: " << commands.at(1)->isValid("lol", gerald) << endl;
    //cout << "leave execute: " << commands.at(1)->execute("lol", &gerald) << endl;

    cout << "rooms matches: " << commands.at(2)->matches("lol") << endl;
    cout << "rooms isValid: " << commands.at(2)->isValid("lol", gerald) << endl;
    //cout << "rooms execute: " << commands.at(2)->execute("lol", &gerald) << endl;

    cout << "who matches: " << commands.at(3)->matches("lol") << endl;
    cout << "who isValid: " << commands.at(3)->isValid("lol", gerald) << endl;
    //cout << "who execute: " << commands.at(3)->execute("lol", &gerald) << endl;

    cout << "help matches: " << commands.at(4)->matches("lol") << endl;
    cout << "help isValid: " << commands.at(4)->isValid("lol", gerald) << endl;
   // cout << "help execute: " << commands.at(4)->execute("lol", &gerald) << endl;

    cout << "quit matches: " << commands.at(5)->matches("lol") << endl;
    cout << "quit isValid: " << commands.at(5)->isValid("lol", gerald) << endl;
    //cout << "quit execute: " << commands.at(5)->execute("lol", &gerald) << endl;

    cout << "whisper matches: " << commands.at(6)->matches("lol") << endl;
    cout << "whisper isValid: " << commands.at(6)->isValid("lol 0 0 0", gerald) << endl;
    //cout << "whisper execute: " << commands.at(6)->execute("lol", gerald) << endl;

    cout << "unreCom matches: " << commands.at(7)->matches("lol") << endl;
    cout << "unreCom isValid: " << commands.at(7)->isValid("lol", gerald) << endl;
    //cout << "unreCom execute: " << commands.at(7)->execute("lol", &gerald) << endl;

    cout << "message matches: " << commands.at(8)->matches("lol") << endl;
    cout << "message isValid: " << commands.at(8)->isValid("lol", gerald) << endl;
    //cout << "message execute: " << commands.at(8)->execute("lol", &gerald) << endl;
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
        //TODO: Spawn Listen thread
        pthread_t p1;
        int rc;
        cout<<"before thread"<<endl;
        cout << commands.size() << endl;
        struct client_args args = {user, commands};
        cout <<"a4\n";
        rc = pthread_create(&p1,NULL,client_run,&args);
        cout <<"a5\n";
        cout<<"after thread"<<endl;
    }
    return 0;

    
}




