#include <iostream>
#include <string>
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
        string firstWord = message.substr(0, message.find(" "));
        if (firstWord.length() >= (message.length() - 1)) {
            return false;
        }
        string secondWord = message.substr(message.find(" "), message.length());
        return ((firstWord.compare(commandSyntax) == 0) && (secondWord.compare("") != 0 ));
    }
    string execute(string message, User *user) override {
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
        //TODO
        return "Executing Rooms";
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
        return true;
    }
    string execute(string message, User *user) override {
        //TODO
        return "Executing Who";
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
        //TODO
        return "Executing Help";
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
        string firstWord = message.substr(0, message.find(" "));
        string secondWord = message.substr(message.find(" "), message.length());
        return ((firstWord.compare(commandSyntax) == 0) && (secondWord.compare("") != 0 ));
    }
    string execute(string message, User *user) override {
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
        return false;
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

