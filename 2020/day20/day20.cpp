#include <algorithm> // reverse()
#include <iostream>
#include <vector>

using namespace std;

// reads from standard input

// each tile is 10x10
// input is 1728 = 144*12 lines
// so the full image is 12x12
// 2^10 = 1024
// number of edges in this problem = 13*12*2 = 312
// so currently assume that no edges repeat
// edit: forgot about rotation/reflection
// number of edges: doubled = 624 - assumption probably still safe?
const int _LEN = 10;
const string _nonUnique = "edges do repeat";

typedef class ImageTile {
    public:
    int ID;
    vector<string> Tiles;
    string northSide = "";
    string eastSide = "";
    string southSide = "";
    string westSide = "";
    ImageTile* tN = NULL;
    ImageTile* tE = NULL;
    ImageTile* tS = NULL;
    ImageTile* tW = NULL;
    
    bool horiz = false;
    bool vert = false;
    int rot = 0;
    
    ImageTile(int id, vector<string> tiles) : ID(id), Tiles(tiles) {
        for (auto s : Tiles) {
            westSide.push_back(s[0]);
            eastSide.push_back(s[_LEN-1]);
        }
        northSide = tiles[0];
        southSide = tiles[_LEN-1];
    }
    ~ImageTile() {}
    void printTile() {
        cout << "id = " << ID << endl;
        cout << "\tnorth = " << northSide << endl;
        cout << "\teast  = " << eastSide << endl;
        cout << "\tsouth = " << southSide << endl;
        cout << "\twest  = " << westSide << endl;
        cout << endl;
    }
    void printTileIDs() {
        cout << "id = " << ID << endl;
        cout << "\tnorth = " << ((tN == NULL) ? -1 : tN->ID) << endl;
        cout << "\teast  = " << ((tE == NULL) ? -1 : tE->ID) << endl;
        cout << "\tsouth = " << ((tS == NULL) ? -1 : tS->ID) << endl;
        cout << "\twest  = " << ((tW == NULL) ? -1 : tW->ID) << endl;
        cout << endl;
    }
    
    // currently may rotate tiles even after being fitted
    // what if, rotate current tile...
    // bad news: rotation is not consistent across all tiles
    // good news: corners are still found
    // bad news: rotation needs to be consistent
    // edit: worst news: this is causing bugs at the very end of part 2 and i no longer have the desire to fix it
    //  specifically: rotating a tile to match an already-rotated tile is fine in every aspect but for accurate rotations
    bool checkMatchingSide(ImageTile* it) {
        // cout << "comparing " << ID << " with " << it->ID << endl;
        for (int i = 0; i < 3; i++) {
            // two flips is just rotation so should only need 3 instead of 2
            if (i>0) flipVert();
            if (i==2) flipHoriz();
            
            for (int j = 0; j < 4; j++) {
                // cout << endl << "\tchecking " << northSide << " with " << it->southSide << endl;
                if (northSide == it->southSide) {
                    if (tN != NULL || it->tS != NULL) throw runtime_error(_nonUnique);
                    tN = it;
                    it->tS = this;
                    return true;
                }
                // cout << "\tchecking " << eastSide << " with " << it->westSide << endl;
                if (eastSide == it->westSide) {
                    if (tE != NULL || it->tW != NULL) throw runtime_error(_nonUnique);
                    tE = it;
                    it->tW = this;
                    return true;
                }
                // cout << "\tchecking " << southSide << " with " << it->northSide << endl;
                if (southSide == it->northSide) {
                    if (tS != NULL || it->tN != NULL) throw runtime_error(_nonUnique);
                    tS = it;
                    it->tN = this;
                    return true;
                }
                // cout << "\tchecking " << westSide << " with " << it->eastSide << endl;
                if (westSide == it->eastSide) {
                    if (tW != NULL || it->tE != NULL) throw runtime_error(_nonUnique);
                    tW = it;
                    it->tE = this;
                    return true;
                }
                rotate();
            }
        }
        
        flipHoriz();
        return false;
    }
    
    // currently doesn't affect Tiles
    void flipHoriz() {
        reverse(northSide.begin(), northSide.end());
        reverse(southSide.begin(), southSide.end());
        string temp = eastSide;
        eastSide = westSide;
        westSide = temp;
        
        ImageTile* tempptr = tE;
        tE = tW;
        tW = tempptr;
        
        horiz = !horiz;
    }
    void flipVert() {
        reverse(eastSide.begin(), eastSide.end());
        reverse(westSide.begin(), westSide.end());
        string temp = northSide;
        northSide = southSide;
        southSide = temp;
        
        ImageTile* tempptr = tN;
        tN = tS;
        tS = tempptr;
        
        vert = !vert;
    }
    void rotate() { // 90deg clockwise
        string temp = eastSide;
        eastSide = northSide;
        northSide = westSide;
        reverse(northSide.begin(), northSide.end());
        
        westSide = southSide;
        southSide = temp;
        reverse(southSide.begin(), southSide.end());
        
        ImageTile* tempptr = tE;
        tE = tN;
        tN = tW;
        tW = tS;
        tS = tempptr;
        
        rot = (rot+1) % 4;
    }
    
} it_t;


// part 2
void flipHorizMap(vector<string>& map) {
    for (int i = 0; i < map.size(); i++) {
        reverse(map[i].begin(), map[i].end());
    }
}

void flipVertMap(vector<string>& map) {
    reverse(map.begin(), map.end());
}

void rotateMap(vector<string>& map) { // 90deg clockwise
    vector<string> cpy(map);
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.size(); j++) {
            cpy[j][map.size()-1-i] = map[i][j];
        }
    }
    map = cpy;
}

void printMap(const vector<string>& map) {
    for (const auto&s : map) cout << s << endl;
    cout << endl;
}

int _BIGLEN = 12;
vector<string> buildFromCorner(const vector<it_t*> &its, it_t* corner) {
    /*  algorithm:
     *  1. rotate corner until top left
     *  2. build map of ids with rotation of other pieces until complete
     *  3. apply each tiles' rotation to their string vectors
     *  4. build final string from updated individual tile layouts (ignore edges)
     */ 
    // currently assuming (hoping) no "corner" has NULL on opposite ends
    while (corner->tN != corner->tW) // both NULL
        corner->rotate();
    // corner->printTileIDs();
    
    // cout << "added " << corner->ID << endl;
    vector<vector<it_t*>> tilesMap = {{corner}};
    it_t* temp;
    it_t* top;
    it_t* next;
    for (int i = 0; i < _BIGLEN; i++) {
        for (int j = 0; j < _BIGLEN-1; j++) {
            temp = tilesMap[i][j];
            top = (i==0) ? NULL : tilesMap[i-1][j+1];
            next = NULL;
            for (auto it : its) {
                if (it->ID == temp->tE->ID) {
                    next = it;
                    break;
                }
            }
            if (next == NULL) throw runtime_error("ID not found");
            
            int rotcount = 0;
            // undo rotations/reflections first to properly recreate on lines
            while (next->rot != 0)
                next->rotate();
            if (next->horiz) next->flipHoriz();
            if (next->vert) next->flipVert();
            cout << next->ID << endl;
            printMap(next->Tiles);
            
            while (next->tW != temp || next->tN != top) {
                if (rotcount >= 12) throw runtime_error("valid reordering not found");
                next->rotate();
                rotcount++;
                if (rotcount == 4) next->flipHoriz();
                if (rotcount == 8) {
                    next->flipVert();
                    next->flipHoriz();
                }
                // cout << next->tW << " " << temp;
            }
            
            cout << rotcount << endl;
            next->printTileIDs();
            tilesMap[i].push_back(next);
            // cout << "added " << next->ID << endl;
        }
        // next line
        if (i == _BIGLEN-1) break;
        vector<it_t*> nextLine;
        temp = NULL;
        top  = tilesMap[i][0];
        next = NULL;
        for (auto it : its) {
            if (it->ID == top->tS->ID) {
                // cout << it->ID << " matches " << top->tS->ID << endl;
                next = it;
                break;
            }
        }
        if (next == NULL) throw runtime_error("ID not found");
        
        int rotcount = 0;
        while (next->tW != temp || next->tN != top) {
            if (rotcount >= 12) throw runtime_error("valid reordering not found");
            if (rotcount == 4) next->flipHoriz();
            if (rotcount == 8) {
                next->flipVert();
                next->flipHoriz();
            }
            next->rotate();
            rotcount++;
        }
        
        // cout << rotcount << endl;
        // next->printTileIDs();
        nextLine.push_back(next);
        tilesMap.push_back(nextLine);
        // cout << "added " << next->ID << endl;
    }
    
    // cout << "tilesMap" << endl;
    for (auto& v : tilesMap) {
        for (auto& it : v) {
            // cout << it->ID << " ";
            // cout << it->ID << " with " << it->rot << " rotations after " << (it->horiz ? "horiz" : (it->vert ? "vert" : "no")) << " flip" << endl;
            // printMap(it->Tiles);
            if (it->rot == 2) {
                flipHorizMap(it->Tiles);
                flipVertMap(it->Tiles);
                continue;
            }
            if (it->horiz) flipHorizMap(it->Tiles);
            else if (it->vert) flipVertMap(it->Tiles);
            for (int i = 0; i < it->rot; i++)
                rotateMap(it->Tiles);
            // printMap(it->Tiles);
        }
        // cout << endl;
    }
    // cout << endl;
    
    vector<string> output;
    for (int i = 0; i < _BIGLEN; i++) {
        for (int j = 0; j < _LEN-2; j++) {
            string mapLine = "";
            for (int k = 0; k < _BIGLEN; k++) {
                mapLine.append(tilesMap[i][k]->Tiles[j+1],1,_LEN-2);
            }
            output.push_back(mapLine);
        }
    }
    
    return output;
}

const int _monsterX[15] = {0,1, 4,5,6,7, 10,11,12,13, 16,17,18,18,19}; 
const int _monsterY[15] = {1,2, 2,1,1,2, 2,1,1,2, 2,1,0,1,1};

void findMonsters(vector<string>& map) {
    /*  algorithm:
     *  1. for each rotation/reflection
     *  2. scan for layout on range x=[0,len-19) and y=[0,len-2)
     *  3. if found replace all characters with 'O'
     *  4. exit loop, if found return new map
     */ 
    flipVertMap(map); // with foreknowledge
    int monsterCount = 0;
    for (int rots = 0; rots < 12; rots++) {
        if (rots%4 == 0) flipHorizMap(map);
        if (rots == 8) flipVertMap(map);
        
        // vector<int> monsterSpots = {};
        for (int j = 0; j < _BIGLEN*(_LEN-2)-2; j++) {
            for (int i = 0; i < _BIGLEN*(_LEN-2)-19; i++) {
                int matches = 0;
                for (int k = 0; k < 15; k++) {
                    if (map[j+_monsterY[k]][i+_monsterX[k]] == '#') matches++;
                    else break;
                }
                if (matches == 15) {
                    // the problem never said they didn't have to overlap though
                    for (int k = 0; k < 15; k++) {
                        map[j+_monsterY[k]][i+_monsterX[k]] = 'O';
                        // monsterSpots.push_back(j+_monsterY[k]);
                        // monsterSpots.push_back(i+_monsterX[k]);
                    }
                    monsterCount++;
                    i += 4; // minimum before new monsters can overlap?
                }
            }
        }
        
        if (monsterCount > 0) break;
        // for (int i = 0; i < monsterSpots.size(); i+=2)
        //     map[monsterSpots[i]][monsterSpots[i+1]] = 'O';
        rotateMap(map);
    }
    cout << monsterCount << endl;
}

int countNonMonster(const vector<string>& map) {
    int count = 0;
    for (const auto& s : map) {
        for (const auto& c : s) {
            if (c == '#') count++;
        }
    }
    return count;
}

int main() {
    vector<it_t*> its;
    string line;
    while (!getline(cin,line).eof()) {
        int id = stoi(line.substr(5,4));
        
        vector<string> lines;
        while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
            lines.push_back(line.substr(0,_LEN));
        } 
        
        its.push_back(new it_t(id, lines));
        // (*(its.end()-1))->printTile();
    }
    
    
    // -------------- part 1
    for (int i = 0; i < its.size(); i++) {
        int matchesMade = 0;
        for (int j = i+1; j < its.size(); j++) {
            if (its[i]->checkMatchingSide(its[j])) matchesMade++;
        }
        // cout << "matches found: " << matchesMade << endl;
    }
    
    // 01
    // 23
    it_t* anyCorner;
    vector<long> cornerids;
    for (auto& i : its) {
        it_t* sides[4] = {i->tN, i->tE, i->tS, i->tW};
        int nullSides = 0;
        for (auto p : sides) {
            if (p == NULL) nullSides++;
        }
        if (nullSides == 2) {
            // cout << "corner found: " << i->ID << endl;
            cornerids.push_back(i->ID);
            anyCorner = i;
        }
    }
    
    long product = cornerids[0] * cornerids[1] * cornerids[2] * cornerids[3];
    cout << product << endl;
    
    // -------------- part 2
    // for testing:
    // _BIGLEN = 3;
    vector<string> fullmap = buildFromCorner(its, anyCorner);
    printMap(fullmap);
    
    // cout << countNonMonster(fullmap) << endl;
    findMonsters(fullmap);
    printMap(fullmap);
    
    int nonMonster = countNonMonster(fullmap);
    cout << nonMonster << endl;
    
    // cleanup
    for (auto i : its) delete i;
    return 0;
}