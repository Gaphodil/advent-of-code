#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

// reads from standard input
// hex grid
// white up, black down
// infinite space
// list of tiles that must be flipped over
// each line specifies a single tile by giving directions from an origin
// e w, ne nw, se sw - no delimiters

// can be simulated with modulo 3 coord system?
// 0,0 has (clockwise from uh, east i guess) 2,0; 1,-1; -1,-1; -2,0; -1,1; 1,1

typedef map<pair<int,int>,bool> hexCoords;

void updateTiles(hexCoords& grid, string line) {
    istringstream ss(line);
    char c;
    bool northWait = false;
    bool southWait = false;
    pair<int,int> cds(0,0); 
    while (ss >> c && !ss.eof()) {
        if (c == 'n') {
            northWait = true;
        }
        else if (c == 's') {
            southWait = true;
        }
        else if (c == 'e') {
            if (northWait) {
                // cout << "ne";
                cds.first += 1;
                cds.second += 1;
            }
            else if (southWait) {
                // cout << "se";
                cds.first += 1;
                cds.second -= 1;
            }
            else {
                // cout << "e";
                cds.first += 2;
            }
            northWait = false;
            southWait = false;
        }
        else if (c == 'w') {
            if (northWait) {
                // cout << "nw";
                cds.first -= 1;
                cds.second += 1;
            }
            else if (southWait) {
                // cout << "sw";
                cds.first -= 1;
                cds.second -= 1;
            }
            else {
                // cout << "w";
                cds.first -= 2;
            }
            northWait = false;
            southWait = false;
        }
    }
    // cout << endl;
    // cout << "coords: " << cds.first << "," << cds.second << endl;
    try {
        grid.at(cds) = !grid.at(cds);
        // cout << "\texisted" << endl;
    }
    catch (const out_of_range& oor) {
        grid[cds] = true;
        // cout << "\tdid not exist" << endl;
    }
}

// THE GAME OF LIFE, HEXAGONAL EDITION
// any BLACK (true) tile with (==0 || >2) adjacent -> false
// any WHITE (false) tile with (==2) adjacent -> true

hexCoords gol(const hexCoords grid) {
    // get set of all adjacent
    set<pair<int,int>> allToEval;
    for (auto p : grid){
        if (!p.second) continue;
        pair<int,int> temp = p.first;
        allToEval.insert(temp); // 0,0
        temp.first += 2;
        allToEval.insert(temp); // 2,0
        temp.first -= 1;
        temp.second += 1;
        allToEval.insert(temp); // 1,1
        temp.second -= 2;
        allToEval.insert(temp); // 1,-1
        temp.first -= 2;
        allToEval.insert(temp); // -1,-1
        temp.second += 2;
        allToEval.insert(temp); // -1,1
        temp.second -= 1;
        temp.first -= 1;
        allToEval.insert(temp); // -2,0
    }
    
    // then evaluate
    hexCoords newGrid(grid);
    for (auto c : allToEval) {
        vector<pair<int,int>> adjs;
        pair<int,int> temp = c;
        temp.first += 2;
        adjs.push_back(temp); // 2,0
        temp.first -= 1;
        temp.second += 1;
        adjs.push_back(temp); // 1,1
        temp.second -= 2;
        adjs.push_back(temp); // 1,-1
        temp.first -= 2;
        adjs.push_back(temp); // -1,-1
        temp.second += 2;
        adjs.push_back(temp); // -1,1
        temp.second -= 1;
        temp.first -= 1;
        adjs.push_back(temp); // -2,0
        
        int numadj = 0;
        for (auto c2 : adjs) {
            try {
                if (grid.at(c2)) numadj++;
            }
            catch (const out_of_range& oor) {;}
        }
        
        bool curstate = false;
        try {
            curstate = grid.at(c);
        }
        catch (const out_of_range& oor) {;}
        
        // cout << "\t" << c.first << "," << c.second << ": " << curstate;
        
        if (curstate) {
            if (numadj == 0 || numadj > 2)
                newGrid[c] = false;
            else newGrid[c] = true;
        }
        else {
            if (numadj == 2)
                newGrid[c] = true;
            else newGrid[c] = false;
        }
        
        // cout << " to " << newGrid[c] << " with " << numadj << " adjacent" << endl;
    }
    
    return newGrid;
}

int countBlack(const hexCoords grid) {
    int count = 0;
    for (auto p : grid)
        if (p.second) count++;
    return count;
}

int main() {
    hexCoords isBlack;
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        updateTiles(isBlack,line);
    }
    
    // updateTiles(isBlack,"esew");
    // updateTiles(isBlack,"nwwswee");
    
    cout << countBlack(isBlack) << endl;
    
    // part 2
    for (int i = 0; i < 100; i++) {
        isBlack = gol(isBlack);
        if (i < 10 || i%10 == 9) {
            cout << "Day " << i+1 << ": " << countBlack(isBlack) << endl;
        }
    }
    
    cout << countBlack(isBlack) << endl;

    return 0;
}