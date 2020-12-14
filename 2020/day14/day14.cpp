#include <cmath> // pow
#include <cstdio> // sscanf
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

// reads from standard input

// theory: 11 with mask 2^1=0 and 2^6=1 (64)
//   11 % 2^2 = 3 -  more than 2^1 so bit overwritten (-2^1)
//   11 % 2^7 = 11 - less than 2^6 so bit overwritten (+2^6)
// result: 11 - 2 + 64 = 73
// 101 with mask 1, 6
//   101 % 4 = 1 so the bit is already 0
//   101 % 128 = 101 so the bit is already 1
//
// 36 bits is still larger than 32bit int so use longs

const int _BITS = 36;

long applyMask(long val, const string mask) {
    long result = val;
    for (int i = 0; i < mask.size(); i++) {
        char c = mask[i];
        if (c == 'X') continue;
        
        long bitval = (long) pow(2,_BITS - i - 1);
        long rem = val % (bitval * 2);
        if ((c == '1') && (rem < bitval)) result += bitval;
        else if ((c == '0') && (rem >= bitval)) result -= bitval;
    }
    return result;
}

// there are few enough X in each mask that this won't be explosive

void part2(map<long,long>* mem, long addr, long val, const string mask) {
    vector<int> inds;
    long changed_addr = addr;
    for (int i = 0; i < mask.size(); i++) {
        char c = mask[i];
        if (c == '0') continue;
        if (c == '1') {
            long bitval = (long) pow(2,_BITS - i - 1);
            long rem = addr % (bitval * 2);
            if (rem < bitval) changed_addr += bitval;
        }
        else if (c == 'X') inds.push_back(i);
    }
    
    // cout << "\tnew base: " << changed_addr << endl;
    // cout << "\tshould have " << (long) pow(2,inds.size()) << " addresses" << endl;
    
    set<long> new_addresses;
    vector<long> base_to_edit = {changed_addr};
    for (int i : inds) {
        int len = base_to_edit.size();
        for (int j = 0; j < len; j++) {
            long temp = base_to_edit[j];
            
            long bitval = (long) pow(2,_BITS - i - 1);
            long rem = temp % (bitval * 2);
            if (rem < bitval) temp += bitval;
            else if (rem >= bitval) temp -= bitval;
            
            base_to_edit.push_back(temp);
            new_addresses.insert(base_to_edit[j]);
            new_addresses.insert(temp);
        }
    }
    
    for (long addr : new_addresses) {
        (*mem)[addr] = val;
        // cout << "mem[" << addr << "] = " << val << endl;
    }
}

int main() {
    map<long,long> memory;
    string line;
    string mask;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        // lines.push_back(line);
        istringstream ss(line);
        string temp;
        ss >> temp;
        if (temp == "mask") {
            ss >> temp >> mask;
            // cout << "mask = " << mask << endl;
        }
        else {
            long addr;
            long val;
            sscanf(line.c_str(),"mem[%ld] = %ld",&addr,&val);
            // cout << "mem[" << addr << "] = " << val << endl;
            
            // part 1
            // memory[addr] = applyMask(val,mask);
            
            // part 2
            part2(&memory, addr, val, mask);
            
        }
    }
    
    // test:
    // part2(&memory, 42, 100, "000000000000000000000000000000X1001X");
    
    long sum = 0;
    for (auto val : memory) {
        sum += val.second;
    }
    cout << sum << endl;

    return 0;
}