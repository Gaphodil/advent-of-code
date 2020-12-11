#include <iostream>
#include <vector>

using namespace std;

// reads from standard input 95x92
// the game of ~life~ change places

// part 1
bool updateLayout(vector<string>* layout) {
    bool changed = false;
    int rows = (*layout).size();
    int cols = (*layout)[0].size();
    
    vector<string> newLayout(*layout);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            bool cup,cdown,cleft,cright;
            cup = i == 0 ? false : true;
            cdown = i == rows-1 ? false : true;
            cleft = j == 0 ? false : true;
            cright = j == cols-1 ? false : true;
            
            int occupied = 0;
            if (cup     && cleft    && (*layout)[i-1][j-1] == '#') occupied++;
            if (cup                 && (*layout)[i-1][j] == '#') occupied++;
            if (cup     && cright   && (*layout)[i-1][j+1] == '#') occupied++;
            if (cleft               && (*layout)[i][j-1] == '#') occupied++;
            if (cright              && (*layout)[i][j+1] == '#') occupied++;
            if (cdown   && cleft    && (*layout)[i+1][j-1] == '#') occupied++;
            if (cdown               && (*layout)[i+1][j] == '#') occupied++;
            if (cdown   && cright   && (*layout)[i+1][j+1] == '#') occupied++;
            
            if ((*layout)[i][j] == 'L' && occupied == 0) {
                changed = true;
                newLayout[i][j] = '#';
            }
            if ((*layout)[i][j] == '#' && occupied >= 4) {
                changed = true;
                newLayout[i][j] = 'L';
            }
        }
    }
    *layout = newLayout;
    return changed;
}

// part 2
int countVis(const vector<string> layout, int i, int j) {
    int rows = layout.size();
    int cols = layout[0].size();
    bool cup,cdown,cleft,cright;
    cup = i == 0 ? false : true;
    cdown = i == rows-1 ? false : true;
    cleft = j == 0 ? false : true;
    cright = j == cols-1 ? false : true;
    
    bool cup2,cdown2,cleft2,cright2;
    int newi, newj;
    int occupied = 0;
    char temp;
    if (cup && cleft) {
        newi = i-1;
        newj = j-1;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cup2 = newi == 0 ? false : true;
            cleft2 = newj == 0 ? false : true;
            if (cup2 && cleft2) {
                newi = newi-1;
                newj = newj-1;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cup) {
        newi = i-1;
        newj = j;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cup2 = newi == 0 ? false : true;
            if (cup2) {
                newi = newi-1;
                newj = newj;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cup && cright) {
        newi = i-1;
        newj = j+1;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cup2 = newi == 0 ? false : true;
            cright2 = newj == cols-1 ? false : true;
            if (cup2 && cright2) {
                newi = newi-1;
                newj = newj+1;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cleft) {
        newi = i;
        newj = j-1;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cleft2 = newj == 0 ? false : true;
            if (cleft2) {
                newi = newi;
                newj = newj-1;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cright) {
        newi = i;
        newj = j+1;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cright2 = newj == cols-1 ? false : true;
            if (cright2) {
                newi = newi;
                newj = newj+1;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cdown && cleft) {
        newi = i+1;
        newj = j-1;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cdown2 = newi == rows-1 ? false : true;
            cleft2 = newj == 0 ? false : true;
            if (cdown2 && cleft2) {
                newi = newi+1;
                newj = newj-1;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cdown) {
        newi = i+1;
        newj = j;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cdown2 = newi == rows-1 ? false : true;
            if (cdown2) {
                newi = newi+1;
                newj = newj;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    if (cdown && cright) {
        newi = i+1;
        newj = j+1;
        temp = layout[newi][newj];
        
        while (temp == '.') {
            cdown2 = newi == rows-1 ? false : true;
            cright2 = newj == cols-1 ? false : true;
            if (cdown2 && cright2) {
                newi = newi+1;
                newj = newj+1;
                temp = layout[newi][newj];
            }
            else break;
        }
        
        if (temp == '#') occupied++;
    }   
    
    return occupied;
}

bool updateLayout2(vector<string>* layout) {
    bool changed = false;
    int rows = (*layout).size();
    int cols = (*layout)[0].size();
    
    vector<string> newLayout(*layout);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char curr = (*layout)[i][j];
            if (curr == '.') continue;
            
            int occupied = countVis(*layout, i, j);
            
            if (curr == 'L' && occupied == 0) {
                changed = true;
                newLayout[i][j] = '#';
            }
            if (curr == '#' && occupied >= 5) {
                changed = true;
                newLayout[i][j] = 'L';
            }
        }
    }
    *layout = newLayout;
    return changed;
}

int countOcc(const vector<string> layout) {
    int rows = layout.size();
    int cols = layout[0].size();
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (layout[i][j] == '#') count++;
        }
    }
    return count;
}

void showLayout(const vector<string> layout) {
    for (string s : layout) cout << s << endl;
}

int main() {
    vector<string> lines;
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        lines.push_back(line);
    }
    
    while (true) {
        // if (!updateLayout(&lines)) break;
        if (!updateLayout2(&lines)) break;
    }
    
    // showLayout(lines);
    cout << countOcc(lines) << endl;
    

    return 0;
}