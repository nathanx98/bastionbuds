#include <iostream>
#include "commands.cpp"
#include <string>
#include <vector>

using namespace std;

int create_thread();

bool clientConnecting = false;


int main() {
    vector<Command*> commands{&join, &leave};

    string word = "woah!";
    cout << word.substr(0, word.find(" ")) << endl;

    cout << "Hello, World!" << endl;
    cout << "join matches: " << commands.at(0)->matches("lol") << endl;
    cout << "join isValid: " << commands.at(0)->isValid("lol") << endl;
    cout << "join execute: " << commands.at(0)->execute("lol", "no") << endl;

    cout << "leave matches: " << commands.at(1)->matches("lol") << endl;
    cout << "leave isValid: " << commands.at(1)->isValid("lol") << endl;
    cout << "leave execute: " << commands.at(1)->execute("lol", "no") << endl;





    clientConnecting = false;

    cout << "Starting server. Press 'CTRL-C to close the server" << endl;
    while(true) {
        /* This is the main thread loop.
        *  This will loop indefinetly, until the user closes using "CTRL-C"
        *  This can be changed later to stop under certain conditions
        */

        if(clientConnecting) {
            //TODO: Lock mutex
            cout << "Client connection to server..." << endl;

            //TODO: Add client to global list of users

            //TODO: Spawn Listen thread
            bool connectionSuccessful = create_thread();

            if (connectionSuccessful)   {cout << "Client successfully connected to sever!" << endl;}
            else                        {cout << "Client could not connect to server!" << endl;}
            //TODO: Unlock Mutex

        }


    }

    return 0;

}



int create_thread() {
    //TODO: Spawn a thread

    return false;
}





