#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

// reads from standard input

int main() {
    int len = 200;
    vector<int> intarr;
    int x;
    while (cin >> x && x != EOF) {
        intarr.push_back(x);
    }

    // part 1
    int first = 0;
    int second = 0;
    bool found = false;
    for (int i = 0; i < len; i++) {
        first = intarr[i];
        for (int j = len-1; j > i; j--) {
            second = intarr[j];
            if (first + second == 2020) {
                found = true;
                break;
            }
        }
        if (found) break;
    }
    
    cout << first << "*" << second << "=" << first*second << endl;
    
    // part 2
    int third = 0;
    found = false;
    for (int i = 0; i < len; i++) {
        first = intarr[i];
        for (int j = len-1; j > i; j--) {
            second = intarr[j];
            for (int k = 0; k < len; k++) {
                third = intarr[k];
                if (third != first && third != second 
                    && first + second + third == 2020) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) break;
    }

    cout << first << "*" << second << "*" << third << 
    "=" << first*second*third << endl;
    
    return 0; 
}