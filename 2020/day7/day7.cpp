#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

// reads from standard input

class BagNode;
map<string, BagNode*> bagMap;
class BagNode {
    string Colour;
    vector<pair<string, int>> Contents;
    set<string> Inside;
    bool didPart1 = false; // avoid infinite recursion
public:
    BagNode(string colour) : Colour(colour) {};
    BagNode(string colour, vector<string> after) : Colour(colour) {
        makeContents(after);
    }
    ~BagNode() {}
    void makeContents(vector<string> contents) {
        for (string s : contents) {
            int amt;
            string p1, p2;
            
            istringstream ss(s);
            ss >> amt >> p1 >> p2;
            
            string c = p1 + " " + p2;
            // cout << "makeshift concat:\t" << c << amt << endl;
            Contents.push_back(make_pair(c,amt));
            
            BagNode* bn;
            try {
                bn = bagMap.at(c);
            }
            catch (const out_of_range& oor) {
                bn = new BagNode(c);
                bagMap[c] = bn;
            }
            bn->addToInside(Colour);
        }
    }
    void addToInside(string container) {
        Inside.insert(container);
    }
    int part1(set<string>* s) {
        if (didPart1) return -1;
        didPart1 = true;
        for (string i : Inside) {
            s->insert(i);
            bagMap[i]->part1(s);
        }
        return s->size();
    }
    int part2() {
        int total = 0;
        for (auto i : Contents) {
            total = total + i.second * (bagMap[i.first]->part2() + 1);
        }
        return total;
    }
};

int main() {
    string line;
    while (!getline(cin,line).eof() && line != "\r" && line != "\n") {
        size_t f = line.find("bags");
        string mainColour = line.substr(0,f-1); // -1 for space
        
        vector<string> contained;
        line = line.substr(f+strlen("bags contain "));
        if (line[0] == 'n');
        else {
            do {
                // cout << line << endl;
                f = line.find("bag");
                if (f == -1) break;
                contained.push_back(line.substr(0,f-1));
                try {
                    if (line[0] == '1') {
                        line = line.substr(f+strlen("bag, "));
                    }
                    else {
                        line = line.substr(f+strlen("bags, "));
                    }
                }
                catch (const out_of_range& oor) {
                    break;
                }
            } while (f != -1);
        }
        
        BagNode* bn;
        try {
            bn = bagMap.at(mainColour);
            bn->makeContents(contained);
        }
        catch (const out_of_range& oor) {
            bn = new BagNode(mainColour,contained);
            bagMap[mainColour] = bn;
        }
    }
    
    // part 1
    set<string> p1s;
    cout << bagMap["shiny gold"]->part1(&p1s) << endl;
    
    // part 2
    cout << bagMap["shiny gold"]->part2() << endl;

    // clean up
    for (auto i : bagMap) {
        delete i.second;
    }

    return 0;
}