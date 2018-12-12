#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>


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
     * @param  a pointer for the room the user should change to
     */
    void setRoom(Room* newRoom) {
        room = newRoom;
    }
    /*
     * @param  a name that the user will be nicknamed
     */
    void setNickname(string newNickname) {
        nickname = newNickname;
    }
    /*
     * @return  the nickname of the user
     */
    string getNickname() {
        return nickname;
    }
    /*
     * @return  the room the user is in
     */
    room *getRoom() {
        return room;
    }
    /*
     * @return  the socket that is associated to this user
     */
    int getSocket() {
        return socket;
    }

    
    int transmit(char* message)
    {
        int output = send(socket,message,strlen(message),0);
        return output;
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
