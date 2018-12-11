#include <iostream>
#include <string>
using namespace std;

class Command {
public:
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
    virtual bool isValid(string command) {return 0;}

    /*
     * @param   command the message sent by a user
     * @param   sender the user who sent the command
     * @return  the string to display to the user who sent the command.
     *          Might end up cutting this out and making the method void
     */
    virtual string execute(string command, string sender) {}
};

class Join: public Command {
public:

    string commandSyntax = "/JOIN";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        string secondWord = message.substr(message.find(" "), message.length());
        return ((firstWord.compare(commandSyntax) == 0) && (secondWord.compare("") != 0 ));
    }
    string execute(string message, string sender) override {
        //mock
        return" Executing Join";
    }
};
Join join = Join();

class Leave: public Command {
public:
    string commandSyntax = "/LEAVE";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(""));
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Leave";
    }
};
Leave leave = Leave();
public:
    string commandSyntax = "/ROOMS";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Rooms";
    }
};
Rooms rooms = Rooms();
public:
    string commandSyntax = "/WHO";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Who";
    }
};
Who who = Who();
public:
    string commandSyntax = "/HELP";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Help";
    }
};
Help help = Help();
public:
    string commandSyntax = "/QUIT";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Quit";
    }
};
Quit quit = Quit();
public:
    string commandSyntax = "/whisper";
    bool matches(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        return (firstWord.compare(commandSyntax) == 0);
    }
    bool isValid(string message) override {
        string firstWord = message.substr(0, message.find(" "));
        string secondWord = message.substr(message.find(" "), message.length());
        return ((firstWord.compare(commandSyntax) == 0) && (secondWord.compare("") != 0 ));
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Whisper";
    }
};
Whisper whisper = whisper();
public:
    string commandSyntax = "/";
    bool matches(string message) override {
        return false;
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Unrecognized command!";
    }
};
UnreCom unreCom = UnreCom();
public:
    string commandSyntax = "";
    bool matches(string message) override {
        return false;
    }
    bool isValid(string message) override {
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Sending message...";
    }
};
Message Message = Message();
