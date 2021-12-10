#include <iostream>
#include <vector>

using namespace std;

// reads from standard input

int main() {
    vector<string> lines;
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        lines.push_back(line);
    }

    return 0;
}