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


class Room;

class User {
private:
    int socket;
    Room *room;

public:

    string nickname;
    //user constructor
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
    Room *getRoom() {
        return room;
    }
    /*
     * @return  the socket that is associated to this user
     */
    int getSocket() {
        return socket;
    }

    /*
     *  @param   the message that needs to be sent to this user
     *  @return  number of bytes sent, -1 if error sending message
     */
    int transmit(char* message)
    {
        int output = send(socket,(char*)message,strlen(message),0);
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
    /*
     *  This is the real fancy s**t and essence of our project
     *  that let you play chess in a chat system!
     */
class ChessBoard {
public:
    int board[8][8] = { {2, 3, 4, 5, 6, 4, 3, 2},
                        {1, 1, 1, 1, 1, 1, 1, 1},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {7, 7, 7, 7, 7, 7, 7, 7},
                        {8, 9,10,11,12,10, 9, 8} };
    string boardString() {
        string returnString = "   1  2  3  4  5  6  7  8\n";
        returnString += " |------------------------|\n";

        for (int i=0; i<8; i++){
            if (i%2 == 0) {returnString += " |███   ███   ███   ███   |\n";}
            else          {returnString += " |   ███   ███   ███   ███|\n";}
            returnString+= to_string(i+1);
            returnString+= "|";
            for(int j=0; j<8; j++) {
                if ((i+j)%2 == 0) {returnString+="█";}
                else {returnString+=" ";}
                switch(board[i][j]) {
                    case 1: returnString+="p";  break;
                    case 2: returnString+="r";  break;
                    case 3: returnString+="n";  break;
                    case 4: returnString+="b";  break;
                    case 5: returnString+="q";  break;
                    case 6: returnString+="k";  break;
                    case 7: returnString+="P";  break;
                    case 8: returnString+="R";  break;
                    case 9: returnString+="N";  break;
                    case 10: returnString+="B"; break;
                    case 11: returnString+="Q"; break;
                    case 12: returnString+="K"; break;
                    default: if ((i+j)%2 == 0) {returnString+="█";}
                             else {returnString+=" ";}
                             break;
                }
                if ((i+j)%2 == 0) {returnString+="█";}
                else {returnString+=" ";}
            }
            if (i%2 == 0) {returnString += "|\n |███   ███   ███   ███   |\n";}
            else          {returnString += "|\n |   ███   ███   ███   ███|\n";}
        }
        returnString += " |------------------------|";
        return returnString;
    }

    /*
     *  @param  currRow and currCol represent the current position.
     *          destRow and destCol represent the destination
     *          you're moving towards
     */

    void movePiece(int currRow, int currCol, int destRow, int destCol){
        if((currRow <= 8 ) && (currCol <= 8 ) && (destRow <= 8 ) && (destCol <= 8 ) &&
        (currRow >= 1 ) && (currCol >= 1 ) && (destRow >= 1 ) && (destCol >= 1 )) {
            board[destRow-1][destCol-1] = board[currRow-1][currCol-1];
            board[currRow-1][currCol-1] = 0;
        }
    }

    //resets the chessboard
    void resetBoard() {
        int newboard[8][8]={{2, 3, 4, 5, 6, 4, 3, 2},
                        {1, 1, 1, 1, 1, 1, 1, 1},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {7, 7, 7, 7, 7, 7, 7, 7},
                        {8, 9,10,11,12,10, 9, 8} };
         copy(&newboard[0][0], &newboard[0][0]+8*8,&board[0][0]);
    }

};


class Room {
private:
    string name;
    ChessBoard chess;

public:
    vector<User*> listOfUsers;

    Room(string roomName) {
        name = roomName;
        chess = ChessBoard();
        chess.resetBoard();
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
    void broadcastInRoom(char* broadcastMessage) {
        for(vector<User*>::iterator it = listOfUsers.begin(); it != listOfUsers.end(); it++) {
            (*it)->transmit(broadcastMessage);
        }
    }
    void chessResetBoard() {
        chess.resetBoard();
    }
    void chessMovePiece(int currRow, int currCol, int destRow, int destCol) {
        chess.movePiece(currRow, currCol, destRow, destCol);
    }
    string chessBoardString() {
        return chess.boardString();
    }
};


