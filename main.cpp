#include <iostream>
#include "commands.cpp"
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<Command*> commands{&join, &leave};

    cout << "Hello, World!" << endl;
    cout << "join matches: " << commands.at(0)->matches("lol") << endl;
    cout << "join isValid: " << commands.at(0)->isValid("lol") << endl;
    cout << "join execute: " << commands.at(0)->execute("lol", "no") << endl;

    cout << "leave matches: " << commands.at(1)->matches("lol") << endl;
    cout << "leave isValid: " << commands.at(1)->isValid("lol") << endl;
    cout << "leave execute: " << commands.at(1)->execute("lol", "no") << endl;
    return 0;
    
}