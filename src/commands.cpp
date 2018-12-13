#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

char *convertString(string str) {
    char *cstring = new char[str.length() + 1];
    strcpy(cstring, str.c_str());
    return cstring;
}

class Command {
public:
    vector<Room*> *roomList;
    string commandSyntax = "";
    /*
     * @param   command the message sent by a user
     * @return  true if the message's first word matches this command and false otherwise.
     *          e.g. if the current command is Join and the first word in the string is
     *          "\Join" then we return true, otherwise return false.
     */
    virtual bool matches(string command) {return 0;}

    /*
     * @param   command the message sent by a user
     * @return  true if the message's arguments matches the format required for this command
     *          and false otherwise, e.g. Join would return true if there is a single argument
     *          after "\Join" specifying the name of a room, and false if there are two arguments
     *          after "\Join".
     */
    virtual bool isValid(string command, User *user) {return 0;}

    /*
     * @param   command the message sent by a user
     * @param   sender the user who sent the command
     */
    virtual void execute(string command, User *user) {}
};

class Join: public Command {
public:
    Join(vector<Room*> *rl) {
        roomList = rl;
    }
    string commandSyntax = "/JOIN";
    bool matches(string message) override {
        cout << "wow\n";
        sleep(1);
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        char* msg = new char[message.size()+1];   //message.size() or size+1?
        strcpy(msg,message.c_str());
        char* firstToken = strtok(msg," ");
        char* secondToken = strtok(NULL," ");      //nickname
        if(secondToken == NULL){
            user->transmit("Not enough arguments");
            return false;
        }
        char* thirdToken = strtok(NULL," ");     //room
        if(thirdToken == NULL)
        {
            user->transmit("Not enough arguments");
            return false;
        }
        if(strtok(NULL," ") != NULL){
            user->transmit("Oh no, you can’t have a space in your nickname.");
            return false;
        }
        if(user->currentRoom() != NULL) {
            user->transmit("Please /LEAVE the current room before using /JOIN again.");
            return false;
        }
        for(int i = 0;i<roomList->size();i++)   //check for duplicated names
        {
            Room* rm = roomList->at(i);
            if(rm->getRoomName().compare(thirdToken) == 0)   //found room
            {
                for(int j = 0;j<rm->listOfUsers.size();j++)
                {
                    if(rm->listOfUsers[j]->getNickname().compare(secondToken)==0)
                    {
                        user->transmit("Uh oh! That nickname already exists here!");
                        return false;
                    }
                }
            }
        }
        return true;
    }
    void execute(string message, User *user) override {
        cout << "Excecuting Join\n";
        if (!isValid(message, user)) {
            return;
        }
        char* msg = new char[message.size()+1];
        strcpy(msg,message.c_str());
        string firstToken = strtok(msg," ");
        string nickname = strtok(NULL," ");
        string roomName = strtok(NULL," ");
        user->setNickname(nickname);
        for(vector<Room*>::iterator it = roomList->begin(); it != roomList->end(); it++) {
            if ((*it)->getRoomName() == roomName) {
                user->setRoom(*it);
                (*it)->addUser(user);
                (*it)->broadcastInRoom(convertString(user->getNickname() + " just joined the room!"));
                return;
            }
        }
        Room *newRoom = new Room(roomName);
        newRoom->addUser(user);
        user->setRoom(newRoom);
        roomList->push_back(newRoom);
        newRoom->broadcastInRoom(convertString(roomName + " didn't exist, so we made it for you! Right now you're the only one here. New friends are sure to join soon!"));
        return;
    }
};
class Leave: public Command {
public:
    string commandSyntax = "/LEAVE";
    Leave(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message, user)) {
            user->transmit("You need to be in a room to leave a room! .0.");
            return;
        }
        cout << "Excecuting Leave\n";
        string returnString = user->getNickname() + " left the room. :(";
        Room *roomToLeave = user->getRoom();
        user->getRoom()->broadcastInRoom(convertString(returnString));
        user->getRoom()->removeUser(user);
        user->setRoom(NULL);
        if (roomToLeave->listOfUsers.size() == 0) {
            vector<Room*>::iterator removePosition = find(roomList->begin(), roomList->end(), roomToLeave);
            if (removePosition != roomList->end()) {
                roomList->erase(removePosition);
            }
        }
        return;
    }
};
class Rooms: public Command {
public:
    string commandSyntax = "/ROOMS";
    Rooms(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return true;
    }
    void execute(string message, User *user) override {
        cout << "Excecuting Rooms\n";
        string returnString = "Current Rooms: ";
        if (roomList->size() <= 0) {
            user->transmit("Sorry, no rooms yet!");
            return;
        }
        for(vector<Room*>::iterator it = roomList->begin(); it != roomList->end(); it++) {
            returnString += (**it).getRoomName() + " ";
        }
        user->transmit(convertString(returnString));
        return;
    }
};
class Who: public Command {
public:
    string commandSyntax = "/WHO";
    Who(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        cout << "Excecuting Who\n";
        if (!isValid(message, user)){
            user->transmit(convertString("You are not currently in a room, and all alone ;_;"));
            return;
        }
        string returnString = "Here are all of the users in " + user->currentRoom()->getRoomName() + ": ";
        if (user->currentRoom()->listOfUsers.size() <= 0) {
            returnString = "Wait a minute, no one is in this room?!? What about you!?!";
            user->transmit(convertString(returnString));
            return;
        }
        for(vector<User*>::iterator it = user->currentRoom()->listOfUsers.begin(); it != user->currentRoom()->listOfUsers.end(); it++) {
            returnString += (**it).getNickname() + " ";
        }
        user->transmit(convertString(returnString));
        return;
    }
};
class Help: public Command {
public:
    string commandSyntax = "/HELP";
    Help(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        cout<<"first line in MATCHES"<<endl;

        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        cout<<"first line in ISVALID"<<endl;

        return true;
    }
    void execute(string message, User *user) override {
        cout<<"first line in execute"<<endl;
        cout << "Excecuting Help\n";
        string returnString = "/JOIN <nickname> <room>: Joins <room> under the name <nickname>. Creates <room> if a room of that name does not exist.\n";
        returnString += "/ROOMS: Lists out all of the current rooms.\n";
        returnString += "/LEAVE: Leaves the current room you are in.\n";
        returnString += "/WHO: Displays all of the users in your current room.\n";
        returnString += "/HELP: Lists all of the commands, as you see here.\n";
        returnString += "/QUIT: Closes the app.\n";
        returnString += "/WHISPER <nickname> <message>: Sends <message> to the person named <nickname> inside your room.";
        returnString += "/CHESSRESET: Resets the room's chessboard.\n";
        returnString += "/CHESSMOVE <Current Column> <Current Row> <Destination Column> <Destination Row>: Moves a piece from current to destination.\n";
        returnString += "/CHESSPRINT: Prints the chessboard for everyone to see.";
        //user->transmit(convertString(returnString));
        //int sock = user->getSocket();
        //send(sock,convertString(returnString),strlen(convertString(returnString)),0);
        user->transmit(convertString(returnString));
        return;
    }
};
class Quit: public Command {
public:
    string commandSyntax = "/QUIT";
    Quit(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return true;
    }
    void execute(string message, User *user) override {
        cout << "Excecuting Quit\n";
        //TODO
        user->transmit("Goodbye!");
    }
};
class Whisper: public Command {
public:
    string commandSyntax = "/WHISPER";
    Whisper(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        if (!user->inRoom()) {
            user->transmit("You whisper to yourself. In order to whisper to someone else, /JOIN a room!");
            return false;
        }
        char* msg = new char[message.size()+1];   //message.size() or size+1?
        strcpy(msg,message.c_str());
        char* firstToken;
        char* secondToken;
        char* thirdToken;
        vector<User*> list = user->getRoom()->listOfUsers;
        firstToken = strtok(msg," ");
        secondToken = strtok(NULL," ");    //name, breaks here
        if(secondToken == NULL)
        {
            user->transmit("Not enough arguments");
            return false;
        }
        thirdToken = strtok(NULL," ");     //actual message
        if(thirdToken == NULL)
        {
            user->transmit("Not enough arguments");
            return false;
        }
        for(int i = 0;i<list.size();i++)
        {
            string name = list[i]->nickname;
            string temp(thirdToken);
            if(name.compare(secondToken) == 0)
                return (temp.compare("") != 0);

        }
        return false;
    }
    void execute(string message, User *user) override {
        cout << "Excecuting Whisper";
        if (!isValid(message, user)) {
            user->transmit("Invalid use! Please use: /WHISPER <nickname> <message>");
            return;
        }
        char* msg = new char[message.size()+1];
        strcpy(msg,message.c_str());
        string firstToken = strtok(msg," ");
        string nickname = strtok(NULL," ");
        string messageToSend = strtok(NULL," ");
        Room *room = user->getRoom();
        for (vector<User*>::iterator it = room->listOfUsers.begin(); it != room->listOfUsers.end(); it++) {
            if ((**it).getNickname() == nickname) {
                string receiverString = "[" + user->getNickname() + " whispers to you] " + messageToSend;
                string senderString = "[You whisper to " + nickname + "] " + messageToSend;
                (**it).transmit(convertString(receiverString));
                user->transmit(convertString(senderString));
                return;
            }
        }
        user->transmit(convertString(nickname + " is not in this room! ;_;"));
        return;
    }
};
class ChessReset: public Command {
public:
    string commandSyntax = "/CHESSRESET";
    ChessReset(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message,user)) {
            user->transmit("You need to be in a room to play chess!");
            return;
        }
        cout << "Excecuting ChessReset\n";
        user->currentRoom()->chessResetBoard();
        user->currentRoom()->broadcastInRoom(convertString(user->currentRoom()->chessBoardString()));
        return;
    }
};
class ChessMove: public Command {
public:
    string commandSyntax = "/CHESSMOVE";
    ChessMove(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        if (!(*user).inRoom()) {
            user->transmit("You need to be in a room to play chess!");
            return false;
        }
        char* msg = new char[message.size()+1];
        try {
            strcpy(msg,message.c_str());
            string firstToken = strtok(msg," ");
            string secondToken = strtok(NULL," ");
            string thirdToken = strtok(NULL," ");
            string forthToken = strtok(NULL," ");
            string fifthToken = strtok(NULL," ");

            int currRow = stoi(secondToken);
            int currCol = stoi(thirdToken);
            int destRow = stoi(forthToken);
            int destCol = stoi(fifthToken);
        }
        catch (const exception &ex) {
            return false;
        }
        return true;
    }
    void execute(string message, User *user) override {
        cout << "Excecuting ChessMove\n";
        if (!isValid(message, user)) {
            return;
        }
        char* msg = new char[message.size()+1];
        strcpy(msg,message.c_str());
        string firstToken = strtok(msg," ");
        string secondToken = strtok(NULL," ");
        string thirdToken = strtok(NULL," ");
        string forthToken = strtok(NULL," ");
        string fifthToken = strtok(NULL," ");
        int currRow = stoi(secondToken);
        int currCol = stoi(thirdToken);
        int destRow = stoi(forthToken);
        int destCol = stoi(fifthToken);
        user->currentRoom()->chessMovePiece(currRow, currCol, destRow, destCol);
        user->currentRoom()->broadcastInRoom(convertString(user->currentRoom()->chessBoardString()));
        return;
    }
};
class ChessPrint: public Command {
public:
    string commandSyntax = "/CHESSPRINT";
    ChessPrint(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message,user)) {
            user->transmit("You need to be in a room to play chess!");
            return;
        }
        cout << "Excecuting ChessPrint\n";
        user->currentRoom()->broadcastInRoom(convertString(user->currentRoom()->chessBoardString()));
        return;
    }
};
class UnreCom: public Command {
public:
    string commandSyntax = "/";
    UnreCom(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        return ((message.length() >= 1) && (message.at(0) == '/'));
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        cout << "Excecuting UnreCom\n";
        user->transmit("The command you attempted does not exist (Oh no!). Type /HELP for a list of usable commands");
        return;
    }
};
class Message: public Command {
public:
    string commandSyntax = "";
    Message(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        return true;
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message, user)) {
            user->transmit("You need to be in a room to talk with people! Try using /JOIN and makes some friends!");
            return;
        }
        cout << "Excecuting Message\n";
        string returnString = "[" + user->getNickname() + "] " + message;
        user->getRoom()->broadcastInRoom(convertString(returnString));
        return;
    }
};
