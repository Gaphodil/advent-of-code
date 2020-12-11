#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// reads from standard input

int highest = -1;

int part1(set<int> s, int rating, int diff1, int diff2, int diff3) {
    int ans;
    
    if (s.empty() && rating == highest) return (diff1 * (diff3+1)); 
    
    set<int> s2(s);
    
    int i = rating+1;
    if (s2.count(i)) {
        s2.erase(i);
        ans = part1(s2,i,diff1+1,diff2,diff3);
        if (ans != -1) return ans;
    }
    
    int j = rating+2;
    s2 = s;
    if (s2.count(j)) {
        s2.erase(j);
        ans = part1(s2,j,diff1,diff2+1,diff3);
        if (ans != -1) return ans;
    }
    
    int k = rating+3;
    s2 = s;
    if (s2.count(k)) {
        s2.erase(k);
        ans = part1(s2,k,diff1,diff2,diff3+1);
        if (ans != -1) return ans;
    }
    
    return -1;
}

int part2(set<int> s, int rating, int tot) {
    int ans;
    
    if (rating == highest) return tot+1; 
    
    set<int> s2(s);
    
    int i = rating+1;
    if (s2.count(i)) {
        s2.erase(i);
        ans = part2(s2,i,tot);
        if (ans != tot) tot = ans;
    }
    
    int j = rating+2;
    s2 = s;
    if (s2.count(j)) {
        s2.erase(j);
        ans = part2(s2,j,tot);
        if (ans != tot) tot = ans;
    }
    
    int k = rating+3;
    s2 = s;
    if (s2.count(k)) {
        s2.erase(k);
        ans = part2(s2,k,tot);
        if (ans != tot) tot = ans;
    }
    
    return tot;
}

map<int,long> waysAtRating;
long part2better(set<int> s, int rating, long tot) {
    long ans;
    
    try {
        return tot+waysAtRating.at(rating);
    }
    catch (const out_of_range& oor) {}
    
    if (rating == highest) {
        waysAtRating[rating] = 1;
        return tot+1; 
    }
    
    set<int> s2(s);
    
    int i = rating+1;
    if (s2.count(i)) {
        s2.erase(i);
        ans = part2better(s2,i,tot);
        if (ans != tot) {
            // cout << ans << endl << tot << endl;
            waysAtRating[i] = ans-tot;
            tot = ans;
        }
    }
    
    int j = rating+2;
    s2 = s;
    if (s2.count(j)) {
        s2.erase(j);
        ans = part2better(s2,j,tot);
        if (ans != tot) {
            waysAtRating[j] = ans-tot;
            tot = ans;
        }
    }
    
    int k = rating+3;
    s2 = s;
    if (s2.count(k)) {
        s2.erase(k);
        ans = part2better(s2,k,tot);
        if (ans != tot) {
            waysAtRating[k] = ans-tot;
            tot = ans;
        }
    }
    
    return tot;
}

int main() {
    int x;
    // int highest = -1;
    set<int> adapters;
    
    while (cin >> x && x != EOF) {
        if (x > highest) highest = x;
        adapters.insert(x);
    }
    
    // int device = highest+3;
    
    cout << part1(adapters, 0,0,0,0) << endl;
    // cout << part2(adapters, 0,0) << endl;
    cout << part2better(adapters, 0,0) << endl;

    return 0;
}