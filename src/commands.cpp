#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

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
     * @return  the string to display to the user who sent the command.
     *          Might end up cutting this out and making the method void
     */
    virtual string execute(string command, User *user) {}
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
        return false;
    }
    string execute(string message, User *user) override {
        string nickname = "";
        string room = "";
        //TODO: Raymond plz do regex, k thx
        //TODO
        return" Executing Join";
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
    string execute(string message, User *user) override {
        //TODO
        return "Executing Leave";
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
    string execute(string message, User *user) override {
        string returnString = "Current Rooms: ";
        if (roomList->size() <= 0) {
            return "Sorry, no rooms yet!";
        }
        for(vector<Room*>::iterator it = roomList->begin(); it != roomList->end(); it++) {
            returnString += (**it).getRoomName() + " ";
        }
        return returnString;
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
    string execute(string message, User *user) override {
        if (!isValid(message, user)){
            return "";
        }
        string returnString = "Here are all of the users in " + user->currentRoom()->getRoomName() + ": ";
        if (user->currentRoom()->listOfUsers.size() <= 0) {
            return "Wait a minute, no one is in this room?!?";
        }
        for(vector<User*>::iterator it = user->currentRoom()->listOfUsers.begin(); it != user->currentRoom()->listOfUsers.end(); it++) {
            returnString += (**it).getNickname() + " ";
        }
        return returnString;
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
    string execute(string message, User *user) override {
        string returnString = "/JOIN <nickname> <room>: Joins <room> under the name <nickname>. Creates <room> if a room of that name does not exist.\n";
        returnString += "/ROOMS: Lists out all of the current rooms.\n";
        returnString += "/LEAVE: Leaves the current room you are in.\n";
        returnString += "/WHO: Displays all of the users in your current room.\n";
        returnString += "/HELP: Lists all of the commands, as you see here.\n";
        returnString += "/QUIT: Closes the app.\n";
        returnString += "/WHISPER <nickname> <message>: Sends <message> to the person named <nickname> inside your room.";
        return returnString;
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
    string execute(string message, User *user) override {
        //TODO
        return "Executing Quit";
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
        return false;
    }
    string execute(string message, User *user) override {
        //TODO: Raymond plz do regex, k thx
        string nickname = "";
        string messageToSend = "";
        //TODO
        return "Executing Whisper";
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
    string execute(string message, User *user) override {
        //TODO
        return "Unrecognized command!";
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
        return true;
    }
    string execute(string message, User *user) override {
        //TODO
        return "Sending message...";
    }
};

