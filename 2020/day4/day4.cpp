#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// reads from standard input

const char* vinit[] = {"byr","iyr","eyr","hgt","hcl","ecl","pid","cid"};

// part 2
bool isColourChar(char ch) {
    static const char colourchars[] = "0123456789abcdef";
    for (int i = 0; i < 16; i++) {
        if (ch == colourchars[i]) return true;
    }
    cout << ch << endl;
    return false;
}

bool isEyeColour(const char* val) {
    static const char* eyecolours[] = {"amb","blu","brn","gry","grn","hzl","oth"};
    for (auto cs : eyecolours) {
        if (strcmp(val,cs) == 0) return true;
    }
    return false;
}

bool isValid(const char* id, const char* val) {
    try {
        if (strcmp(id,vinit[0]) == 0) {
            int year = atoi(val);
            // cout << year << endl;
            if (year >= 1920 && year <= 2002) return true;
            return false;
        }
        
        if (strcmp(id,vinit[1]) == 0) {
            int year = atoi(val);
            if (year >= 2010 && year <= 2020) return true;
            return false;
        }
        
        if (strcmp(id,vinit[2]) == 0) {
            int year = atoi(val);
            if (year >= 2020 && year <= 2030) return true;
            return false;
        }
        
        if (strcmp(id,vinit[3]) == 0) {
            int height = atoi(val);
            if (strstr(val,"cm") != NULL) {
                if (height >= 150 && height <= 193) return true;
            }
            else if (strstr(val,"in") != NULL) {
                if (height >= 59 && height <= 76) return true;
            }
            return false;
        }
        
        if (strcmp(id,vinit[4]) == 0) {
            if (val[0] != '#') return false;
            int i = 1;
            char c = val[i];
            // cout << c << endl;
            while (c != '\0') {
                if (!isColourChar(c)) return false;
                i++;
                c = val[i];
            }
            if (i > 7) return false;
            return true;
        }
        
        if (strcmp(id,vinit[5]) == 0) {
            return isEyeColour(val);
        }
        
        if (strcmp(id,vinit[6]) == 0) {
            if (strlen(val) != 9) return false;
            return true;
        }
        
        if (strcmp(id,vinit[7]) == 0) { return true; }
        
        return false;
    }
    catch (...) {return false;}
    return false;
}

int main() {
    vector<vector<string>> inp;
    vector<string> pport;
    string line;
    while (!getline(cin,line).eof()) {
        if ("\n" == line || "\r" == line) { // stupid carriage returns
            inp.push_back(pport);
            pport.clear();
        }
        else {
            pport.push_back(line.substr(0,line.size()-1));
        }
    }
    // for (int i = 0; i < inp.size(); i++) {
    //     vector<string> p = inp[i];
    //     for (int j = 0; j < p.size(); j++)
    //         cout << p[j] << endl;
    //     cout << endl;
    // }
    
    // part 1
    unsigned valid = 0;
    for (auto p : inp) {
        unsigned ppfields = 0;
        bool goodP = true;
        for (auto s : p) {
            istringstream ss(s);
            char id[4];
            char val[256];
            while (!ss.getline(id, 4, ':').eof()) {
                // cout << id << ' ';
                for (auto cs : vinit) {
                    if (strcmp(cs,id) == 0) {
                        // part 2
                        ss.getline(val,256,' ');
                        // cout << "check " << id << ":\t" << val << endl;
                        
                        // ignore cid
                        if (strcmp(cs,"cid") == 0) break;
                        
                        if (!isValid(id,val)) {
                            // cout << "invalid" << endl;
                            goodP = false;
                        }
                        ppfields++;
                        break;
                    }
                }
                if (goodP == false) {
                    // cout << "invalid, ignoring rest" << endl;
                    break;
                }
            }
            if (goodP == false) break;
        }
        if (goodP && ppfields >= 7) {
            valid++;
            // cout << "valid!" << endl;
        }
        // cout << endl;
        // cout << endl << ppfields << valid << endl;
    }
    cout << valid << endl;
    
    return 0;
}