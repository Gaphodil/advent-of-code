#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// ignoring standard input

int part1(const vector<int> ids, const int timestamp) {
    int ans = -1;
    int earliest = timestamp + 1000;
    for (auto i : ids) {
        int busarrive = i * (timestamp / i + 1);
        // cout << busarrive << endl;
        if (busarrive < earliest) {
            earliest = busarrive;
            ans = i * (earliest - timestamp);
        }
    }
    return ans;
}

long get_lcm(const vector<long> v, long min) {
    long lcm = *max_element(v.begin(), v.end());
    lcm = lcm / min;
    while (true) {
        bool islcm = true;
        for (long l : v) {
            if (lcm % (l/min) == 0) continue;
            islcm = false;
            break;
        }
        if (islcm) return lcm*min;
        lcm++;
    }
}

long get_ans(vector<long> d, const vector<long> b) {
    // long ans = *max_element(d.begin(), d.end());
    long ans = 415579909629976;
    while (true) {
        bool isans = true;
        for (int i = 0; i < d.size(); i++) {
            if (d[i] >= ans) continue;
            cout << ans << endl;
            isans = false;
            while (d[i] < ans) d[i] += b[0] * b[i+1];
            if (d[i] > ans) ans = d[i];
        }
        if (isans) return ans;
    }
}

int main() {
    // int timestamp;
    // cin >> timestamp;
    
    // vector<int> busses = {13, 37, 449, 29, 19, 23, 773, 41, 17};
    // int x;
    // while (cin >> x && x != EOF) {
    //     busses.push_back(x);
    //     cout << x << endl;
    // }
    
    // cout << part1(busses, timestamp) << endl;
    
    vector<long> busses = {13, 37, 449, 29, 19, 23, 773, 41, 17};
    vector<int> waits =     {6,  5,    1,  16, 3,  7,  9,  6};
    vector<int> waits2 = {6,6+5,6+5+1,6+5+1+16,6+5+1+16+3,6+5+1+16+3+7,6+5+1+16+3+7+9,6+5+1+16+3+7+9+6};
    
    // test data
    // vector<long> busses = {7,13,59,31,19};
    // vector<int> waits =    {0,2,1,0};
    // vector<int> waits2 = {0,2,3,3};
    
    /* 
    13 x x x x x x 37 implies 13a + 7 = 37b
    one equation with two unknowns per pair (not necessarily consecutive)
    total of 9 unknowns, 36 pairs/equations
    
    idea: start with highest id -> lowest unknown, make all others +- terms of that
    23 x x x x x x x 773 -> 23f + 8 = 773g
    so ((773*g)/(23*f+8) == 1) && ((773*g)%(23*f+8) == 0)
    
    idea: something with lcm???
    19 x x x 23 -> 19e + 4 = 23f; e,f = 1
    so diff(e,f) == 0 and final answer % lcm(19,23) == 0
    but how does diff affect answer?
    
    note: are all these ids prime? yes - so lcm is always the multiple
    
    first example: a=8, b=3; slope = 13/37 so a=8+37=45, b=3+13=16
    lcm(13,37) == 481; 13(8) == 104; 13(45) == 585 == 104+481
    so (final answer - initial displacement) % (lcm) == 0
    
    idea: create equation by pairing first with every other id
    solve for lowest a, record displacement
        how to solve for a? keep going until id2*b % id1 == 0?
    record lcm(id1,id2) == id1*id2
    then final answer = lcm(all ids) + lcm(all initial displacments)
    incorrect: far too high
    
    oops it happens earlier: work with initial displacements, but
    not actually lcm: instead, add value (id1*id2)
    
    still too brute force, so: gave up and looked at hints
    shows Chinese Remainder Theorem, which i should have known 
    considering i just finished a course on rings/fields
    
    actually whatever, top "solution" on reddit is writing a wolframalpha question so
    (t+0)mod 13=0,(t+7)mod 37=0,(t+13)mod 449=0,(t+15)mod 29=0,
    (t+32)mod 19=0,(t+36)mod 23=0,(t+44)mod 773=0,(t+54)mod 41=0,
    (t+61)mod 17=0
    
    also apparently i had an error in my "waits" vector the entire time 
    with final value doubled, whoops
    */
    
    // actual part 2
    // long lcm_ids = 1;
    // for (long id : busses) 
    //     lcm_ids *= id;
    // cout << "lcm_ids = " << lcm_ids << endl;
    
    vector<long> disps;
    for (int i = 0; i < waits.size(); i++) {
        long temp = busses[i+1];
        long diff = waits2[i] + i + 1;
        long m = 1;
        while ((temp < diff) || ((temp - diff) % busses[0] != 0)) {
            m++;
            temp = busses[i+1] * m;
        }
        cout << "(" << busses[i+1] << "*" << m << ") - " << diff << " \t= " << temp-diff
            << " % " << busses[0] << " \t== 0" << endl;
        disps.push_back(temp - diff);
    }
    
    // long lcm_disps = get_lcm(disps, busses[0]);
    long lcm_disps = get_ans(disps, busses);
    cout << "lcm_disps = " << lcm_disps << endl;
    // cout << lcm_ids - lcm_disps << endl;
    
    // brute force part 2
    // long surely = 100000000000000;
    // for (long j = 13 * (surely/13 + 1); true; j += 13) {
    //     bool found = true;
    //     for (int i = 0; i < waits.size(); i++) {
    //         if ((surely+waits[i]+1) % busses[i+1] == 0) continue;
    //         found = false;
    //         break;
    //     }
    //     if (found) {
    //         cout << j << endl;
    //         break;
    //     }
    // }

    return 0;
}