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

// A function for converting C++ strings into C strings
char *convertString(string str) {
    char *cstring = new char[str.length() + 1];
    strcpy(cstring, str.c_str());
    return cstring;
}

class Command {
public:
    // A pointer to the global list of rooms, which can be accessed by all commands and clients.
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
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }

    bool isValid(string message, User *user) override {
        char* msg = new char[message.size()+1];   //message.size() or size+1?
        strcpy(msg,message.c_str());

        // Because of the way strtok works, we need to go through tokens individually and
        // check that we have enough arguments

        char* firstToken = strtok(msg," ");         // /JOIN

        char* secondToken = strtok(NULL," ");       // nickname
        if(secondToken == NULL){
            user->transmit("Not enough arguments");
            return false;
        }

        char* thirdToken = strtok(NULL," ");        // room
        if(thirdToken == NULL)
        {
            user->transmit("Not enough arguments");
            return false;
        }

        // More than three arguments means something has a space
        if(strtok(NULL," ") != NULL){
            user->transmit("Oh no, you can’t have a space in your nickname or room.");
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

        char* msg = new char[message.size()+1];     // strtok requires C-style strings
        strcpy(msg,message.c_str());

        string firstToken = strtok(msg, " ");
        string nickname   = strtok(NULL, " ");
        string roomName   = strtok(NULL, " ");

        user->setNickname(nickname);

        // Find the relevant room and add our user.
        for(vector<Room*>::iterator it = roomList->begin(); it != roomList->end(); it++) {
            if ((*it)->getRoomName() == roomName) {
                user->setRoom(*it);
                (*it)->addUser(user);
                (*it)->broadcastInRoom(convertString(user->getNickname() + " just joined the room!"));
                return;
            }
        }

        // No relevant room exists, create a new one.
        Room *newRoom = new Room(roomName);
        newRoom->addUser(user);
        user->setRoom(newRoom);

        roomList->push_back(newRoom);
        newRoom->broadcastInRoom(convertString(
            roomName + " didn't exist, so we made it for you! Right now you're the only one here. "
            + "New friends are sure to join soon!"));
        return;
    }
};

class Leave: public Command {
public:
    Leave(vector<Room*> *rl) {
        roomList = rl;
    }

    string commandSyntax = "/LEAVE";

    // Matches if the first word is /LEAVE
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }

    // User must already be in the room
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

        // Tell everyone that the user is leaving
        user->getRoom()->broadcastInRoom(convertString(returnString));
        user->getRoom()->removeUser(user);
        user->setRoom(NULL);

        // If the user who just left is the last user,
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
    Rooms(vector<Room*> *rl) {
        roomList = rl;
    }

    string commandSyntax = "/ROOMS";

    // Matches if the first word is /ROOMS
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }

    // Cannot be invalid, don't care about trailing extra words
    bool isValid(string message, User *user) override {
        return true;
    }

    void execute(string message, User *user) override {
        cout << "Excecuting Rooms\n";

        // build returnString iteratively
        string returnString = "Current Rooms: ";

        if (roomList->size() <= 0) {
            user->transmit("Sorry, no rooms yet!");
            return;
        }

        for(vector<Room*>::iterator it = roomList->begin(); it != roomList->end(); it++) {
            returnString += (**it).getRoomName() + " ";
        }

        // Send this command to only this one user
        user->transmit(convertString(returnString));
        return;
    }
};

class Who: public Command {
public:
    Who(vector<Room*> *rl) {
        roomList = rl;
    }

    string commandSyntax = "/WHO";

    // Matches if the first word is /WHO
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }

    // Only valid if the user is currently in a room
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }

    void execute(string message, User *user) override {
        cout << "Excecuting Who\n";
        if (!isValid(message, user)){
            user->transmit(convertString("You are not currently in a room, and all alone ;_;"));
            return;
        }

        // start building message to display to user
        string returnString = "Here are all of the users in " + user->currentRoom()->getRoomName() + ": ";
        if (user->currentRoom()->listOfUsers.size() <= 0) {
            returnString = "Wait a minute, no one is in this room?!? What about you!?!";
            user->transmit(convertString(returnString));
            return;
        }

        // Add all users in this room to returnString
        for(vector<User*>::iterator it = user->currentRoom()->listOfUsers.begin();
            it != user->currentRoom()->listOfUsers.end();
            it++) {
            returnString += (**it).getNickname() + " ";
        }

        // Send to user
        user->transmit(convertString(returnString));
        return;
    }
};

class Help: public Command {
public:
    Help(vector<Room*> *rl) {
        roomList = rl;
    }

    string commandSyntax = "/HELP";

    // Matches if first word is /HELP
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }

    // Don't care about trailing words so this is always valid
    bool isValid(string message, User *user) override {
        return true;
    }

    void execute(string message, User *user) override {
        cout << "Excecuting Help\n";

        // Hardcoded description of all the commands. Hardcoded because we don't need it anywhere else.
        string returnString = "/JOIN <nickname> <room>: Joins <room> under the name <nickname>. Creates <room> if a room of that name does not exist.\n";
        returnString += "/ROOMS: Lists out all of the current rooms.\n";
        returnString += "/LEAVE: Leaves the current room you are in.\n";
        returnString += "/WHO: Displays all of the users in your current room.\n";
        returnString += "/HELP: Lists all of the commands, as you see here.\n";
        returnString += "/QUIT: Closes the app.\n";
        returnString += "/WHISPER <nickname> <message>: Sends <message> to the person named <nickname> inside your room.\N";
        returnString += "/CHESSRESET: Resets the room's chessboard.\n";
        returnString += "/CHESSMOVE <Current Column> <Current Row> <Destination Column> <Destination Row>: Moves a piece from current to destination.\n";
        
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
        //The user can always use quit
        return true;
    }
    void execute(string message, User *user) override {
        cout << "Excecuting Quit\n";
        //Sends /QUIT to the client, informing the client that it is okay to quit now.
        user->transmit("/QUIT");
        return;
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
        char* msg = new char[message.size()+1];   //message.size()+1
        strcpy(msg,message.c_str());
        char* firstToken;
        char* secondToken;
        char* thirdToken;
        vector<User*> list = user->getRoom()->listOfUsers;
        firstToken = strtok(msg," ");
        secondToken = strtok(NULL," ");    //name, breaks here
        if(secondToken == NULL)
        {
            //This happens whisper is missing the second argument
            user->transmit("Not enough arguments");
            return false;
        }
        thirdToken = strtok(NULL," ");     //actual message
        if(thirdToken == NULL)
        {
            //This happens whisper is missing the last argument
            user->transmit("Not enough arguments");
            return false;
        }
        for(int i = 0;i<list.size();i++)
        {
            string name = list[i]->nickname;
            string temp(thirdToken);
            //This will return false if the user is not in the room, or no name was given.
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
        string messageToSend = message.substr(nickname.length()+firstToken.length()+1, message.length());
        Room *room = user->getRoom();
        //Search the room for the targeted user, and send them the whisper message
        for (vector<User*>::iterator it = room->listOfUsers.begin(); it != room->listOfUsers.end(); it++) {
            if ((**it).getNickname() == nickname) {
                string receiverString = "[" + user->getNickname() + " whispers to you] " + messageToSend;
                string senderString = "[You whisper to " + nickname + "] " + messageToSend;
                (**it).transmit(convertString(receiverString));
                user->transmit(convertString(senderString));
                return;
            }
        }
        //Informs the user that that person is not in the room
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
        //The user is only allowed to run chess commands when they are in a room.
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message,user)) {
            //Inform the user they must be in a room to play chess.
            user->transmit("You need to be in a room to play chess!");
            return;
        }
        cout << "Excecuting ChessReset\n";
        user->currentRoom()->chessResetBoard();
        //Print the chessboard
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
            //Inform the user they must be in a room to play chess.
            user->transmit("You need to be in a room to play chess!");
            return false;
        }
        char* msg = new char[message.size()+1];
        try {
            //Check to see if the user sent 4 int tokens for the chess piece to move by
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
        //Get the poistion the chess piece starts at and mvoes to
        string firstToken = strtok(msg," ");
        string secondToken = strtok(NULL," ");
        string thirdToken = strtok(NULL," ");
        string forthToken = strtok(NULL," ");
        string fifthToken = strtok(NULL," ");
        int currRow = stoi(secondToken);
        int currCol = stoi(thirdToken);
        int destRow = stoi(forthToken);
        int destCol = stoi(fifthToken);
        //Move the chess piece
        user->currentRoom()->chessMovePiece(currRow, currCol, destRow, destCol);
        //Print the chess board
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
        //ChessPrint will be valid if the user is in a room
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message,user)) {
            //Inform the user they must be in a room to play chess.
            user->transmit("You need to be in a room to play chess!");
            return;
        }
        cout << "Excecuting ChessPrint\n";
        //Print the chess board
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
        //An UnrecCommand will always match the criteria if its tarts with /
        return ((message.length() >= 1) && (message.at(0) == '/'));
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {

        cout << "Excecuting UnreCom\n";
        //UnreCom will transmit to the user that they typed an incorrect command
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
        //Message will always be in the correct format, and match
        return true;
    }
    bool isValid(string message, User *user) override {
        //Message will always be valid if the user is inside a room
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        if (!isValid(message, user)) {
            //Tell the user if they are not in a room.
            user->transmit("You need to be in a room to talk with people! Try using /JOIN and makes some friends!");
            return;
        }
        cout << "Excecuting Message\n";
        //Transmit the message to everyone in the room.
        string returnString = "[" + user->getNickname() + "] " + message;
        user->getRoom()->broadcastInRoom(convertString(returnString));
        return;
    }
};
