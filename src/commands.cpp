#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>

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
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        //TODO: Raymond plz do regex, k thx
        /*
        string firstWord = message.substr(0, message.find(" "));
        if (firstWord.length() >= (message.length())) {
            cout << "1\n";
            return false;
        }
        string secondWord = message.substr(message.find(" "), message.length());
        string thirdWord = message.substr(message.find(" "), message.length());
        thirdWord = message.substr(0, message.find(" "));
        secondWord = secondWord.substr(0,message.find(" "));
        if (secondWord.length() + firstWord.length() >= (message.length())) {
            cout << "2\n";
            return false;
        }
        if (secondWord.length() + firstWord.length() + thridWord >= (message.length())) {
            cout << "3\n";
            return false;
        }
        return ((secondWord.compare("") != 0 ) && (thirdWord.compare("") != 0 ));
        */
        char* msg = new char[message.size()+1];   //message.size() or size+1?
        strcpy(msg,message.c_str());
        string firstToken = strtok(msg," ");
        string secondToken = strtok(NULL," ");    //nickname
        string thirdToken = strtok(NULL," ");     //room
        if(strtok(NULL," ") != NULL){
            user->transmit("Oh no, you can’t have a space in your nickname.");
            return false;
        }
        if(user->currentRoom() != NULL) {
            user->transmit("Please \LEAVE the current room before using \JOIN again.");
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
                        user->transmit("Uh oh! There’s already a here!\n");
                        return false;
                    }
                }
            }
        }
        return true;
    }
    void execute(string message, User *user) override {
        string nickname = "";
        string room = "";
        //TODO: Raymond plz do regex, k thx
        //TODO
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
        return (firstWord.compare(""));
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        string returnString = user->getNickname() + " left the room. :(";
        user->getRoom()->broadcastInRoom(convertString(returnString));
        user->getRoom()->removeUser(user);
        user->setRoom(NULL);
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
        if (!isValid(message, user)){
            user->transmit("You are not currently in a room, and all alone ;_;");
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
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        return true;
    }
    void execute(string message, User *user) override {
        string returnString = "/JOIN <nickname> <room>: Joins <room> under the name <nickname>. Creates <room> if a room of that name does not exist.\n";
        returnString += "/ROOMS: Lists out all of the current rooms.\n";
        returnString += "/LEAVE: Leaves the current room you are in.\n";
        returnString += "/WHO: Displays all of the users in your current room.\n";
        returnString += "/HELP: Lists all of the commands, as you see here.\n";
        returnString += "/QUIT: Closes the app.\n";
        returnString += "/WHISPER <nickname> <message>: Sends <message> to the person named <nickname> inside your room.";
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
        //TODO
        return;
    }
};
class Whisper: public Command {
public:
    string commandSyntax = "/whisper";
    Whisper(vector<Room*> *rl) {
        roomList = rl;
    }
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message, User *user) override {
        //TODO: Raymond plz do regex, k thx
    /*
        string firstWord = message.substr(0, message.find(" "));
        string secondWord = message.substr(message.find(" "), message.length());
        return ((firstWord.compare(commandSyntax) == 0) && (secondWord.compare("") != 0 ));
        */
        char* msg = new char[message.size()+1];   //message.size() or size+1?
        strcpy(msg,message.c_str());
        string firstToken = strtok(msg," ");
        string secondToken = strtok(NULL," ");    //name
        string thirdToken = strtok(NULL," ");     //actual message
        vector<User*> list = user->getRoom()->listOfUsers;
        for(int i = 0;i<list.size();i++)
        {
            string name = list[i]->nickname;
            if(name.compare(secondToken) == 0) break;
            return false;
        }
        return (thirdToken.compare("") != 0);
    }
    void execute(string message, User *user) override {
        //TODO: Raymond plz do regex, k thx
        string nickname = "";
        string messageToSend = "";
        //TODO
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
        return true;
    }
    void execute(string message, User *user) override {
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
        return false;
    }
    bool isValid(string message, User *user) override {
        return (*user).inRoom();
    }
    void execute(string message, User *user) override {
        string returnString = "[" + user->getNickname() + "] " + message;
        user->getRoom()->broadcastInRoom(convertString(returnString));
        return;
    }
};

