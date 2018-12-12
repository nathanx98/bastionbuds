#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


class User;




class Room {
private:
    string name;

public:
    vector<User*> listOfUsers;

    Room(string roomName) {
        name = roomName;
    }

    /*
     * @param  a pointer for the user that is being added
     */
    void addUser(User* newUser) {
        listOfUsers.push_back(newUser);
    }

    /*
     * @param  a pointer for the user that is being removed
     */
    void removeUser(User* userToRemove) {
        vector<User*>::iterator removePosition = find(listOfUsers.begin(), listOfUsers.end(), userToRemove);
        if (removePosition != listOfUsers.end()) {
            listOfUsers.erase(removePosition);
        }
    }


    /*
     * @return  the name of the server
     */
    string getRoomName() {
        return name;
    }
};


class User {
private:
    int socket;
    Room *room;

public:

    string nickname;

    User(int sock) {
        socket = sock;
        nickname = "";
        room = NULL;
    }

    /*
     * @return  the socket that is associated to this user
     */
    int getSocket() {
        return socket;
    }

    /*
     * @return  the pointer to the room the user is in,
     *          and NULL if the user is not in a room
     */
    Room *currentRoom() {
        return room;
    }

    /*
     * @return  true if the user is in a room
     *          and false otherwise, e.g. room == NULL
     */
    bool inRoom() {
        return (room != NULL);
    }

};
