#include <iostream>
#include <sstream>
#include <vector>

#define getNewState(b,n) ((b)?((((n)>1)&&((n)<4))?true:false):(((n)==3)?true:false))

using namespace std;

// reads from standard input
// conway again? but 3d this time - was this in 2019 too?

void printCycle(const vector<vector<string>>* space) {
    int len = space->size();
    for (int i = 0; i < len; i++) {
        cout << "z = " << i - len/2 << endl;
        for (string s : space->at(i)) {
            cout << s << endl;
        }
        cout << endl;
    }
    cout << endl;
}

void printCycle2(const vector<vector<vector<string>>>* space) {
    int len1 = space->size();
    int len2 = space->at(0).size();
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            cout << "z = " << j - len2/2 << ", w = " << i - len1/2 << endl;
            for (string s : space->at(i)[j]) {
                cout << s << endl;
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;
}

// ---------------

int getActive(const vector<vector<string>>* space, int x, int y, int z) {
    int active = 0;
    for (int i = x-1; i <= x+1; i++) {
        if ((i < 0) || (i >= (int)(space->at(0)[0].size()))) continue;
        
        for (int j = y-1; j <= y+1; j++) {
            if ((j < 0) || (j >= (int)(space->at(0).size()))) continue;
            
            for (int k = z-1; k <= z+1; k++) {
                if ((k < 0) || (k >= (int)(space->size()))) continue;
                
                // forgot to not check itself
                if ((x==i)&&(y==j)&&(z==k)) continue;
                
                // cout << "\t\tchecking "<<space->at(k)[j][i]<<" at " << i<<","<<j<<","<<k<<endl;
                if (space->at(k)[j][i] == '#') active++;
            }
        }
    }
    return active;
}

void doCycle(vector<vector<string>>* space) {
    vector<vector<string>> newSpace;
    
    // now i've learned: size_type is an UNSIGNED integer (why wouldn't it be?)
    for (int k = -1; k < (int)(space->size()+1); k++) {
        vector<string> newDim;
        for (int j = -1; j < (int)(space->at(0).size()+1); j++) {
            ostringstream oss;
            for (int i = -1; i < (int)(space->at(0)[0].size()+1); i++) {
                int active = getActive(space, i, j, k);
                
                bool curr;
                if ((k == -1) || (k == space->size()) 
                    || (j == -1) || (j == space->at(0).size())
                    || (i == -1) || (i == space->at(0)[0].size())) {
                    curr = false;
                }
                else curr = ((space->at(k)[j][i] == '#') ? true : false);
                // cout <<"found " << active << " neighbours" << endl;
                // cout <<"\tat " << curr << ": " << i <<","<<j<<","<<k<<endl<<endl;
                char cube = getNewState(curr, active) ? '#' : '.';
                oss << cube;
            }
            // now i've learned: if you initialize an ostringstream with a string,
            // it doesn't edit that string object, just copies its contents
            string s = oss.str();
            // cout << "s=" << s ;
            // cout << endl << endl;
            
            newDim.push_back(s);
        }
        newSpace.push_back(newDim);
    }
    
    *space = newSpace;
    // printCycle(space);
}

int countActive(const vector<vector<string>>* space) {
    int count = 0;
    for (auto& v : *space) {
        for (auto& s : v) {
            for (auto& c : s) {
                if (c == '#') count++;
            }
        }
    }
    return count;
}

// ---------------

int getActive2(const vector<vector<vector<string>>>* space, int x, int y, int z, int w) {
    int active = 0;
    for (int i = x-1; i <= x+1; i++) {
        if ((i < 0) || (i >= (int)(space->at(0)[0][0].size()))) continue;
        
        for (int j = y-1; j <= y+1; j++) {
            if ((j < 0) || (j >= (int)(space->at(0)[0].size()))) continue;
            
            for (int k = z-1; k <= z+1; k++) {
                if ((k < 0) || (k >= (int)(space->at(0).size()))) continue;
                
                for (int l = w-1; l <= w+1; l++) {
                    if ((l < 0) || (l >= (int)(space->size()))) continue;
                    if ((x==i)&&(y==j)&&(z==k)&&(l==w)) continue;
                    if (space->at(l)[k][j][i] == '#') active++;
                }
            }
        }
    }
    return active;
}

void doCycle2(vector<vector<vector<string>>>* space) {
    vector<vector<vector<string>>> newSpace;
    
    for (int l = -1; l < (int)(space->size()+1); l++) {
        vector<vector<string>> newDim2;
        for (int k = -1; k < (int)(space->at(0).size()+1); k++) {
            vector<string> newDim;
            for (int j = -1; j < (int)(space->at(0)[0].size()+1); j++) {
                ostringstream oss;
                for (int i = -1; i < (int)(space->at(0)[0][0].size()+1); i++) {
                    int active = getActive2(space, i, j, k, l);
                    
                    bool curr;
                    if ((l == -1) || (l == space->size()) 
                        || (k == -1) || (k == space->at(0).size())
                        || (j == -1) || (j == space->at(0)[0].size())
                        || (i == -1) || (i == space->at(0)[0][0].size())) {
                        curr = false;
                    }
                    else curr = ((space->at(l)[k][j][i] == '#') ? true : false);
                    // cout <<"found " << active << " neighbours" << endl;
                    // cout <<"\tat " << curr << ": " << i <<","<<j<<","<<k<<","<<l<<endl<<endl;
                    
                    char cube = getNewState(curr, active) ? '#' : '.';
                    oss << cube;
                }
                string s = oss.str();
                newDim.push_back(s);
            }
            newDim2.push_back(newDim);
        }
        newSpace.push_back(newDim2);
    }
    
    *space = newSpace;
    // printCycle2(space);
}

int countActive2(const vector<vector<vector<string>>>* space) {
    int count = 0;
    for (auto& v1 : *space) {
        for (auto& v2 : v1) {
            for (auto& s : v2) {
                for (auto& c : s) {
                    if (c == '#') count++;
                }
            }
        }
    }
    return count;
}



int main() {
    vector<string> lines;
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        lines.push_back(line);
    }

    vector<vector<string>> dim;
    // test
    // dim.push_back({".#.","..#","###"});
    dim.push_back(lines);
    
    // part 2
    vector<vector<vector<string>>> dim2;
    dim2.push_back(dim);
    
    for (int i = 0; i < 6; i++) {
        // cout << "cycle " << i+1 << endl;
        // doCycle(&dim);
        doCycle2(&dim2);
    }
    
    // int active = countActive(&dim);
    int active = countActive2(&dim2);
    cout << active << endl;
    
    return 0;
}