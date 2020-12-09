#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

// reads from standard input

bool isValid(vector<long> parts, long next) {
    for (long i : parts) {
        for (long j : parts) {
            if (i+j == next && i != j) return true;
        }
    }
    return false;
}

int main() {
    const long part1ans = 31161678;
    
    long x;
    // vector<long> last25;
    // int to25 = 0;
    
    vector<long> allparts;
    
    while (cin >> x && x != EOF) {
        // if (to25 < 25) {
        //     last25.push_back(x);
        //     to25++;
        // }
        // else {
        //     if (!isValid(last25, x)) {
        //         cout << x << endl;
        //         break;
        //     }
        //     last25[to25 % 25] = x; // replace
        //     to25++;
        // }
        
        if (x == part1ans) break;
        allparts.push_back(x);
    }
    
    // part 2
    int len = 2;
    const int end = allparts.size(); 
    bool found = false;
    
    while (true) {
        for (int i = 0; i < end-len; i++) {
            auto first = allparts.begin()+i;
            auto second = allparts.begin()+i+len;
            long accum = accumulate(first, second, 0);
            // cout << accum << endl;
            
            if (accum == part1ans) {
                cout << accum << "=" << part1ans << endl;
                cout << *min_element(first, second) + *max_element(first, second) << endl;
                found = true;
                break;
            }
        }
        len++; 
        if (found || len > allparts.size()) break;
    }

    return 0;
}