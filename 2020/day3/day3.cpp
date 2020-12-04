#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

// reads from standard input

// part 2
int part2(vector<string> map, int right, int down) {
    int row = -down;
    int col = -right;
    int trees = 0;
    for (int i = 0; i < map.size(); i += down) {
        row += down;
        col = (col+right) % map[0].size();
        if (map[row][col] == '#') {
            trees ++;
        }
    }
    return trees;
}

int main() {
    string x;
    vector<string> map;
    while (cin >> x && x != "") {
        map.push_back(x);
    }
    // for (int i = 0; i < map.size(); i++)
    //     cout << map[i] << endl;
    
    // part 1
    int row = -1;
    int col = -3;
    int trees = 0;
    for (int i = 0; i < map.size(); i++) {
        row += 1;
        col = (col+3) % map[0].size();
        if (map[row][col] == '#') {
            trees ++;
            // map[row][col] = 'X';
        }
    }
    // for (int i = 0; i < map.size(); i++)
    //     cout << map[i] << endl;
    
    // cout << trees << endl;
    
    cout << part2(map,1,1) * part2(map,3,1) * part2(map,5,1) * part2(map,7,1) * part2(map,1,2) << endl;

    return 0;
}