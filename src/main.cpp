#include <iostream>
#include "room_user.cpp"
#include "commands.cpp"
#include <string>
#include <vector>

using namespace std;

int create_thread();

bool clientConnecting = false;
vector<Room*> roomList;

int main() {
    Join join = Join(&roomList);
    Leave leave = Leave(&roomList);
    Rooms rooms = Rooms(&roomList);
    Who who = Who(&roomList);
    Help help = Help(&roomList);
    Quit quit = Quit(&roomList);
    Whisper whisper = Whisper(&roomList);
    UnreCom unreCom = UnreCom(&roomList);
    Message message = Message(&roomList);

    vector<Command*> commands{&join, &leave};
    User gerald = User(12);
    Room room = Room("the_wOw_room");

    cout << "room name: " << room.getRoomName() << endl;
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.addUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.removeUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;
    room.removeUser(&gerald);
    cout << "room size: " << room.listOfUsers.size() << endl;

    string word = "woah!";
    cout << word.substr(0, word.find(" ")) << endl;

    cout << "Hello, World!" << endl;
    cout << "join matches: " << commands.at(0)->matches("lol") << endl;
    cout << "join isValid: " << commands.at(0)->isValid("lol", &gerald) << endl;
    cout << "join execute: " << commands.at(0)->execute("lol", &gerald) << endl;

    cout << "leave matches: " << commands.at(1)->matches("lol") << endl;
    cout << "leave isValid: " << commands.at(1)->isValid("lol", &gerald) << endl;
    cout << "leave execute: " << commands.at(1)->execute("lol", &gerald) << endl;





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





