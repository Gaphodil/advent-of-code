#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// reads from standard input

int main() {
    // part 1
    // string line;
    // int qsum = 0;
    // int arr[26] = {0};
    // while (!getline(cin,line).eof()) {
    //     if ("\n" == line || "\r" == line) { // stupid carriage returns
    //         for (int i = 0; i < 26; i++) {
    //             qsum += arr[i];
    //             arr[i] = 0;
    //         }
    //     }
    //     else {
    //         istringstream ss(line);
    //         char c;
    //         while (ss >> c && c != '\n' && c != '\r') {
    //             arr[c - 'a'] = 1;
    //         }
    //     }
    // }
    
    // part 2
    string line;
    int qsum = 0;
    int group_count = 0;
    int arr[26] = {0};
    while (!getline(cin,line).eof()) {
        if ("\n" == line || "\r" == line) { // stupid carriage returns
            for (int i = 0; i < 26; i++) {
                if (arr[i] == group_count)
                    qsum ++;
                arr[i] = 0;
            }
            group_count = 0;
        }
        else {
            istringstream ss(line);
            char c;
            while (ss >> c && c != '\n' && c != '\r') {
                arr[c - 'a'] ++;
            }
            group_count++;
        }
    }
    
    cout << qsum << endl;

    return 0;
}