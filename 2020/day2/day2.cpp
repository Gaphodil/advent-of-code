#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// reads from standard input

// part 1
int part1() {
    int valid = 0;
    int min;
    int max;
    char in;
    string pass;
    for (int i = 0; i < 1000; i++) {
        cin >> min;
        cin.ignore(1,'-');
        cin >> max >> in;
        cin.ignore(1,':');
        cin >> pass;
        
        // cout << min << "-" << max << " " << in << ": " << pass << endl;
        int count = 0;
        // string tosearch = string(1,in);
        // auto it = find(pass.begin(), pass.end(), tosearch);
        // while (it != pass.end()) {
        
        auto found = pass.find(in);
        while (found != -1) {
            count++;
            // it = find(it, pass.end(), tosearch);
            found = pass.find(in,found+1);
        }
        if (count >= min && count <= max)
            valid++;
    }
    return valid;
}

// part 2
int part2() {
    int valid = 0;
    int min;
    int max;
    char in;
    string pass;
    for (int i = 0; i < 1000; i++) {
        cin >> min;
        cin.ignore(1,'-');
        cin >> max >> in;
        cin.ignore(1,':');
        cin >> pass;
        
        // cout << min << "-" << max << " " << in << ": " << pass << endl;
        bool first = (pass.at(min-1) == in);
        bool second = (max-1 < pass.length() && pass.at(max-1) == in);
        // cout << pass.at(min-1) << pass.at(max-1) << in << endl;
        if (first) {
            if (second) {
                continue;
            }
            valid++;
        }
        else if (second)
            valid++;
    }
    return valid;
}

int main() {
    // cout << part1() << endl;
    cout << part2() << endl;
    return 0;
}