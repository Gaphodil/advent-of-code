#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// reads from standard input

// part 1
int part1(vector<string> passes) {
    int max = -1;
    for (auto pass : passes) {
        int row = 0;
        for (int i = 0; i < 7; i++) {
            char c = pass[i];
            if (c == 'B') {
                int power = 6-i;
                row += pow(2,power);
            }
        }
        
        int col = 0;
        for (int i = 7; i < 10; i++) {
            char c = pass[i];
            if (c == 'R') {
                int power = 9-i;
                col += pow(2,power);
            }
        }
        
        int id = row*8 + col;
        if (id > max) max = id;
    }
    return max;
}

// part 2
int secondpass(vector<string> passes, int max, int min) {
    int taken[max-min+1] = {0};
    for (auto pass : passes) {
        int row = 0;
        for (int i = 0; i < 7; i++) {
            char c = pass[i];
            if (c == 'B') {
                int power = 6-i;
                row += pow(2,power);
            }
        }
        
        int col = 0;
        for (int i = 7; i < 10; i++) {
            char c = pass[i];
            if (c == 'R') {
                int power = 9-i;
                col += pow(2,power);
            }
        }
        
        int id = row*8 + col;
        taken[id-min] = 1;
    }
    
    for (int i = 0; i < max-min+1; i++) {
        if (taken[i] == 0) return i+min;
    }
    return -1;
}

int part2(vector<string> passes) {
    int max = -1;
    int min = 128*10;
    for (auto pass : passes) {
        int row = 0;
        for (int i = 0; i < 7; i++) {
            char c = pass[i];
            if (c == 'B') {
                int power = 6-i;
                row += pow(2,power);
            }
        }
        
        int col = 0;
        for (int i = 7; i < 10; i++) {
            char c = pass[i];
            if (c == 'R') {
                int power = 9-i;
                col += pow(2,power);
            }
        }
        
        int id = row*8 + col;
        if (id > max) max = id;
        if (id < min) min = id;
    }
    return secondpass(passes, max, min);
}

int main() {
    vector<string> lines;
    string line;
    while (!getline(cin,line).eof()) {
        // cout << line << endl;
        lines.push_back(line);
    }
    
    // cout << part1(lines) << endl;
    cout << part2(lines) << endl;

    return 0;
}