#include <iostream>
#include <string>
using namespace std;

class Command {
public:
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
    bool matches(string message) override {
        // mock
        return true;
    }
    bool isValid(string message) override {
        //mock
        return false;
    }
    string execute(string message, string sender) override {
        //mock
        return" Executing Join";
    }
};
Join join = Join();

class Leave: public Command {
public:
    bool matches(string message) override {
        // mock
        return false;
    }
    bool isValid(string message) override {
        //mock
        return true;
    }
    string execute(string message, string sender) override {
        //mock
        return "Executing Leave";
    }
};
Leave leave = Leave();
