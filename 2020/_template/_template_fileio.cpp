#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// reads from file input

int main() {
    istream input;
    input.open("input.txt");
    
    
    int x;
    while (input >> x && x != EOF) {
        cout << x << endl;
    }
    input.close();
    
    
    return 0;
}