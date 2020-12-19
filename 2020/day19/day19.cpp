#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

// reads from standard input
bool ruleCmp(const string &a, const string &b) {
    istringstream s1(a), s2(b);
    int ai, bi;
    s1 >> ai;
    s2 >> bi;
    return (ai < bi);
}

// there are only two characters: a and b
// there are thus two rules with " in them, which makes it very easy to locate and hardcode
int _aLoc = 77;
int _bLoc = 91;

// each rule has at most one OR character |, so there are only two possibilities
// for rules without OR, will only have 1 or 2 rules (not 3 as in given example)
// for rules with OR, will be divided into 2 and 2
//      one single exception of 1 and 1
// this is... too slow.........
bool verifyMessage(const vector<string>& rules, const string &message, vector<int> activeRules) {
    vector<int> newActiveRules = activeRules;
    // cout << endl;
    for (int i = 0; i < activeRules.size(); i++) {
        int ind = activeRules[i];
        // cout << "ind = " << ind << endl;
        if (ind == _aLoc || ind == _bLoc) continue;
        
        // parse rules, return OR on recursive calls with new ruleset(s)
        int out[4];
        int ign;
        if (5 == sscanf(rules[ind].c_str(), "%d: %d %d | %d %d", &ign, out, out+1, out+2, out+3)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 5 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            auto nAR2(newActiveRules);
            
            posIt = newActiveRules.begin()+i;
            newActiveRules.insert(posIt, out, out+2);
            
            posIt = nAR2.begin()+i;
            nAR2.insert(posIt, out+2,out+4);
            
            return (verifyMessage(rules,message,newActiveRules) || verifyMessage(rules,message,nAR2));
        }
        else if (3 == sscanf(rules[ind].c_str(), "%d: %d | %d", &ign, out, out+1)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 5 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            auto nAR2(newActiveRules);
            
            posIt = newActiveRules.begin()+i;
            newActiveRules.insert(posIt, out[0]);
            
            posIt = nAR2.begin()+i;
            nAR2.insert(posIt, out[0]);
            
            return (verifyMessage(rules,message,newActiveRules) || verifyMessage(rules,message,nAR2));
        }
        else if (3 == sscanf(rules[ind].c_str(), "%d: %d %d", &ind, out, out+1)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 3 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            newActiveRules.insert(posIt, out[1]);
            newActiveRules.insert(posIt, out[0]);
            
            return (verifyMessage(rules,message,newActiveRules));
        }
        else if (2 == sscanf(rules[ind].c_str(), "%d: %d", &ind, out)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 2 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            newActiveRules.insert(posIt, out[0]);
            
            return (verifyMessage(rules,message,newActiveRules));
        }
        throw runtime_error("sscanf failed at: " + rules[ind]);
        
    }
    // all elements must now be 77 or 91
    string s = "";
    for (auto &i : activeRules) {
        s.push_back((i==_aLoc) ? 'a' : 'b');
    }
    // cout << "\tcomparing message " << message << endl << "\twith rule output " << s << endl;
    // cout << "\t" << message.size() << "\t" << s.size() << endl;
    // cout << "\t" << (message == s) << endl;
    return (message == s);
}

// alternate idea: precompute every single valid output and just iterate through
// surely it can't be that many right???????????????????? (haha)
// at least there's no loops
// edit: okay there are loops but surely it won't be too bad if i just add a max length right?
//   right?????????????????????
//   edit: evidently 2^89 is in fact a very large number
int _cLoc = -1;
int _dLoc = -2;

void buildOutput(const vector<string>& rules, set<string> &output, vector<int> activeRules, int maxlen = 89) {
    vector<int> newActiveRules = activeRules;
    // cout << endl;
    for (int i = 0; i < activeRules.size(); i++) {
        int ind = activeRules[i];
        // cout << "ind = " << ind << endl;
        if (ind == _aLoc || ind == _bLoc || ind == _cLoc || ind == _dLoc) continue;
        
        // parse rules, return OR on recursive calls with new ruleset(s)
        int out[5];
        int ign;
        // if (6 == sscanf(rules[ind].c_str(), "%d: %d %d | %d %d %d", &ign, out, out+1, out+2, out+3, out+4)) {
        //     if (ign != ind) throw runtime_error("bad sort");
        //     // // cout << 6 << endl;
            
        //     auto posIt = newActiveRules.begin()+i;
        //     newActiveRules.erase(posIt);
        //     // auto nAR2(newActiveRules);
            
        //     posIt = newActiveRules.begin()+i;
        //     // newActiveRules.insert(posIt, out, out+2);
        //     // newActiveRules.insert(posIt, out[1]);
        //     newActiveRules.insert(posIt, _cLoc);
        //     // newActiveRules.insert(posIt, out[0]);
            
        //     // posIt = nAR2.begin()+i;
        //     // nAR2.insert(posIt, out+2,out+5);
            
        //     buildOutput(rules,output,newActiveRules);
        //     // if (activeRules.size() >= maxlen) return;
        //     // buildOutput(rules,output,nAR2);
        //     // return;
            
        //     return;
        // }
        if (5 == sscanf(rules[ind].c_str(), "%d: %d %d | %d %d", &ign, out, out+1, out+2, out+3)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 5 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            auto nAR2(newActiveRules);
            
            posIt = newActiveRules.begin()+i;
            newActiveRules.insert(posIt, out, out+2);
            
            posIt = nAR2.begin()+i;
            nAR2.insert(posIt, out+2,out+4);
            
            buildOutput(rules,output,newActiveRules);
            if (activeRules.size() >= maxlen) return;
            buildOutput(rules,output,nAR2);
            return;
        }
        // if (4 == sscanf(rules[ind].c_str(), "%d: %d | %d %d", &ign, out, out+1, out+2)) {
        //     // if (ign != ind) throw runtime_error("bad sort");
        //     // // cout << 4 << endl;
            
        //     auto posIt = newActiveRules.begin()+i;
        //     newActiveRules.erase(posIt);
        //     // auto nAR2(newActiveRules);
            
        //     posIt = newActiveRules.begin()+i;
        //     // newActiveRules.insert(posIt, out[0]);
        //     newActiveRules.insert(posIt, _dLoc);
            
        //     // posIt = nAR2.begin()+i;
        //     // nAR2.insert(posIt, out+1,out+3);
            
        //     buildOutput(rules,output,newActiveRules);
        //     // if (activeRules.size() >= maxlen) return;
        //     // buildOutput(rules,output,nAR2);
        //     return;
        // }
        else if (3 == sscanf(rules[ind].c_str(), "%d: %d | %d", &ign, out, out+1)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 5 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            auto nAR2(newActiveRules);
            
            posIt = newActiveRules.begin()+i;
            newActiveRules.insert(posIt, out[0]);
            
            posIt = nAR2.begin()+i;
            nAR2.insert(posIt, out[1]);
            
            buildOutput(rules,output,newActiveRules);
            if (activeRules.size() >= maxlen) return;
            buildOutput(rules,output,nAR2);
            return;
        }
        else if (3 == sscanf(rules[ind].c_str(), "%d: %d %d", &ind, out, out+1)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 3 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            newActiveRules.insert(posIt, out, out+2);
            
            buildOutput(rules,output,newActiveRules);
            return;
        }
        else if (2 == sscanf(rules[ind].c_str(), "%d: %d", &ind, out)) {
            if (ign != ind) throw runtime_error("bad sort");
            // cout << 2 << endl;
            
            auto posIt = newActiveRules.begin()+i;
            newActiveRules.erase(posIt);
            newActiveRules.insert(posIt, out[0]);
            
            buildOutput(rules,output,newActiveRules);
            return;
        }
        throw runtime_error("sscanf failed at: " + rules[ind]);
        
    }
    // all elements must now be 77 or 91
    string s = "";
    for (auto &i : activeRules) {
        s.push_back((i==_aLoc) ? 'a' : 'b');
    }
    
    // cout << s << endl;
    output.insert(s);
    if (output.size() % 100000 == 0) cout << "size: " << output.size() << endl;
    return;
}

set<string> getAllPossible(const vector<string>& rules, int start = 0) {
    set<string> output;
    buildOutput(rules,output,{start});
    return output;
}

bool altVerifyMessage(const set<string>& output, const string& message) {
    return (output.find(message) != output.end());
}

bool altVerifyMessage2(const set<string>& output,const set<string>& lr,const set<string>& rr, const string& message) {
    return (output.find(message) != output.end());
    // if (output.find(message) != output.end()) return true;
    
    // deal with loops - input only has 8, 11 exactly once each (besides loops)
    // ACTUALLY "0: 8 11" means the input follows a VERY strict guide:
    //      1 or more from rule {42} which is contained entirely within lr
    //      0 or more from rule {31} which is contained entirely within rr
    //  this means the current structure isn't necessary
    //  wow i really should've taken the hint given by the problem instructions
    //  moving on to version 3!
    
    
    // for (auto s : output) {
    //     int cpos = s.find('c');
    //     int dpos = s.find('d');
    //     if (cpos != -1) ;
    //     else continue;
        
    //     if (message.find(s.substr(0,cpos)) == 0) {
    //         string half2 = message.substr(cpos);
    //         // find lr elements until end - all elements are 8 characters
    //         int newpos = 0;
    //         bool min1 = false;
    //         while (newpos != -1) {
    //             bool found = false;
    //             for (auto l : lr) {
    //                 newpos = half2.find(l);
    //                 if (newpos == 0) {
    //                     found = true;
    //                     min1 = true;
    //                     break;
    //                 }
    //             }
    //             if (found) {
    //                 half2 = half2.substr(8);
    //             }
    //         }
    //         if (!min1) return false;
    //         // now find rr elements
    //         newpos = 0;
    //         // min1 not entirely required based on 8 or 11
    //     }
    // }
    
    // return false;
}

bool altVerifyMessage3(const set<string>& lr, const set<string>& rr, string message) {
    // the input follows a VERY strict guide:
    //      1 or more from rule {42} which is contained entirely within lr
    //      0 or more from rule {31} which is contained entirely within rr
    // edit: i misread the input; it is ONE or more from rule 31
    // edit2: i also misread that it is TWO or more from rule 42
    
    // find lr elements until end - all elements are 8 characters
    int newpos = 0;
    bool found;
    int compCount = 0;
    
    while (newpos != -1 && message.size() > 0) {
        found = false;
        for (auto l : lr) {
            newpos = message.find(l);
            if (newpos == 0) {
                found = true;
                compCount++;
                break;
            }
        }
        if (found) {
            message = message.substr(8);
        }
        // cout << message << newpos << endl;
        else break;
    }
    if (compCount < 2) return false;
    
    // now find rr elements
    newpos = 0;
    compCount = 0;
    
    while (newpos != -1 && message.size() > 0) {
        found = false;
        for (auto r : rr) {
            newpos = message.find(r);
            if (newpos == 0) {
                found = true;
                compCount++;
                break;
            }
        }
        if (found) {
            message = message.substr(8);
        }
        // cout << message << newpos << endl;
        else return false;
    }
    if (compCount < 1) return false;
    
    return true;
}

bool altVerifyMessage4(const set<string>& lr, const set<string>& rr, string message) {
    // the same thing but a different approach?
    //   edit: finally realized that for r right components there must be at least l+1 left
    if (message.size() % 8 != 0) return false;
    vector<string> components;
    while (message.size() != 0) {
        components.push_back(message.substr(0,8));
        message = message.substr(8);
        // cout << *(components.end()-1) << " ";
    }
    // cout << endl;
    // string breakdown = "";
    
    bool switched = false;
    int lcount(0), rcount(0);
    for (int i = 0; i < components.size(); i++) {
        string c = components[i];
        if (i < 2) {
            // breakdown.push_back('l');
            if (lr.find(c) == lr.end()) return false;
            lcount++;
        }
        else {
            if (!switched) {
                if (lr.find(c) == lr.end()) switched = true;
                // else breakdown.push_back('l');
                else lcount++;
            }
            if (switched) { // not else because run on same component
                // breakdown.push_back('r');
                if (rr.find(c) == rr.end()) return false;
                rcount++;
            }
        }
    }
    // cout << "\t" << breakdown << endl << endl;
    // it can only switch after 2, and must successfully switch to be valid
    return (switched && (lcount > rcount));
}



int main() {
    // for testing:
    // _aLoc = 1;
    // _bLoc = 4;
    
    vector<string> rules;
    
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        rules.push_back(line);
    }
    sort(rules.begin(), rules.end(), ruleCmp);
    // for (auto s : rules) cout << s << endl;
    
    int validCount = 0;
    // set<string> allOuts = getAllPossible(rules);
    // cout << "got set of outputs" << endl;
    set<string> leftRepeat = getAllPossible(rules,42);
    set<string> rightRepeat = getAllPossible(rules,31);
    
    // ---- this reveals that every element is exactly 8 characters
    // cout << "leftRepeat: " << endl;
    // for (auto s : leftRepeat) cout << "\t" << s << endl;
    // cout << "rightRepeat: " << endl;
    // for (auto s : rightRepeat) cout << "\t" << s << endl;
    // cout << endl;
    
    // out of curiosity, checked for intersection if there might have been bugs there: looks like no
    // vector<string> intersect;
    // set_intersection(leftRepeat.begin(),leftRepeat.end(),rightRepeat.begin(), rightRepeat.end(),intersect.begin());
    // for (auto s : intersect) cout << s << endl;
    
    // just in case i screwed up the function though:
    // for (auto l : leftRepeat) if (rightRepeat.find(l) != rightRepeat.end()) cout << l << endl;
    // for (auto r : rightRepeat) if (leftRepeat.find(r) != leftRepeat.end()) cout << r << endl;
    
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        // messages.push_back(line);
        // if (verifyMessage(rules,line.substr(0,line.size()-1),{0})) validCount++;
        
        // if (altVerifyMessage(allOuts,line.substr(0,line.size()-1))) validCount++;
        // if (altVerifyMessage2(allOuts,leftRepeat,rightRepeat,line.substr(0,line.size()-1))) validCount++;
        // if (altVerifyMessage3(leftRepeat,rightRepeat,line.substr(0,line.size()-1))) validCount++;
        if (altVerifyMessage4(leftRepeat,rightRepeat,line.substr(0,line.size()-1))) validCount++;
        
        // cout << "total: " << validCount << "\tverified " << line.substr(0,line.size()-1) << endl;
    }
    
    cout << validCount << endl;

    return 0;
}