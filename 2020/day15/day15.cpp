#include <iostream>
#include <map>
#include <vector>

using namespace std;

// reads from standard input

map<int,int> numAndLastTurn;
// const int _TURNCOUNT = 2020;
const int _TURNCOUNT = 30000000;

int main() {
    vector<int> input = {9,19,1,6,0,5,4};
    // vector<int> input = {0,3,6};
    
    for (int i = 0; i < input.size(); i++) {
        numAndLastTurn[input[i]] = i+1;
    }
    
    int last = *(input.end());
    int lastSeen = -1;
    
    for (int turn = input.size()+1; turn <= _TURNCOUNT; turn++) {
        int turnVal;
        
        if (lastSeen == -1) turnVal = 0;
        else turnVal = numAndLastTurn[last] - lastSeen;
        
        if (numAndLastTurn[turnVal] != 0) lastSeen = numAndLastTurn[turnVal];
        else lastSeen = -1;
        
        numAndLastTurn[turnVal] = turn;
        
        if (turn%1000000 == 0)
            cout << "turn\t" << turn << ":\t" << turnVal << endl;
        
        last = turnVal;
    }
    
    cout << last << endl;
    
    return 0;
}