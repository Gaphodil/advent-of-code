#include <cstdio> // sscanf, sprintf
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// reads from standard input

string str(int i) {
    char cs[5];
    sprintf(cs, "%d", i);
    string s(cs);
    return s;
}

class TicketField {
    string Name;
    int a1, a2, b1, b2;
public:
    TicketField(string name, int x1, int x2, int y1, int y2) 
        : Name(name), a1(x1), a2(x2), b1(y1), b2(y2) {}
    ~TicketField() {}
    bool isValid(int val) {
        return (val >= a1 && val <= a2) || (val >= b1 && val <= b2);
    }
    string getString() {
        return Name + ": " + str(a1) + "-" + str(a2) + " or " + str(b1) + "-" + str(b2);
    }
    string getName() {return Name;}
};

// part 2 - matrix has unique property: 
// every field has distinct number of possibilities
// returns field type in order of 
void sat(vector<vector<bool>>* mat, vector<int>* res) {
    bool resComp = true;
    for (auto i : *res) {
        if (i == -1) {
            resComp = false;
            break;
        }
    }
    if (resComp) return;
    
    for (int i = 0; i < mat->size(); i++) {
        if ((*res)[i] != -1) continue;
        
        int numOfBools = 0;
        int firstBool = -1;
        for (int j = 0; j < mat->size(); j++) {
            bool b = (*mat)[i][j];
            if (b) {
                numOfBools++;
                firstBool = j;
            }
            if (numOfBools > 1) break;
        }
        if (numOfBools == 1) {
            // cout << "pos " << i << " is field " << firstBool << endl;
            (*res)[i] = firstBool;
            for (int j = 0; j < mat->size(); j++) {
                (*mat)[j][firstBool] = false;
            }
            sat(mat,res);
            return;
        }
    }
    throw runtime_error("incorrect assumption in SAT");
}

int main() {
    vector<TicketField*> fields;
    
    string line;
    // rules
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        char name[40];
        int a1, a2, b1, b2;
        sscanf(line.c_str(), "%[^:]: %d-%d or %d-%d",name,&a1,&a2,&b1,&b2);
        // cout << a1 << a2 << b1 << b2 << endl;
        
        string sname(name);
        TicketField* tf = new TicketField(sname,a1,a2,b1,b2);
        fields.push_back(tf);
        // cout << tf->getString() << endl;
    }
    
    // my ticket
    vector<int> myTicket;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        if (line.find("your") != -1) continue;
        // cout << "my ticket: " << line << endl;
        
        istringstream ss(line);
        bool midline = true;
        while (midline && !ss.eof()) {
            int val;
            char end;
            ss >> val;
            ss >> end;
            if (end == '\r' || end == '\n') midline = false;
            
            myTicket.push_back(val);
        }
    }
    
    int errorRate = 0;
    
    vector<vector<bool>> determination;
    vector<bool> baseDet;
    for (auto f : fields) {
        baseDet.push_back(true);
    }
    for (auto f: fields) {
        determination.push_back(baseDet);
    }
    // making sure copy by value
    // determination[3][0] = false;
    // for (auto v : determination) {
    //     for (auto b : v) cout << b;
    //     cout << endl;
    // }
    
    // nearby tickets
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        if (line.find("nearby") != -1) continue;
        
        bool oneIsValid; 
        bool midline = true;
        istringstream ss(line);
        while (midline && !ss.eof()) {
            int val;
            char end;
            ss >> val;
            ss >> end;
            if (end == '\r' || end == '\n') midline = false;
            // cout << val << ",";
            
            oneIsValid = false;
            for (auto field : fields) {
                if (field->isValid(val)) {
                    oneIsValid = true;
                    break;
                }
            }
            // part 1
            if (!oneIsValid) {
                // errorRate += val;
                // cout << "invalid line" << endl;
                break;
            }
        }
        // part 2-1: collect info on which value can have which field
        if (!oneIsValid) continue;
        
        ss.str(line);
        ss.clear();
        
        midline = true;
        int posInLine = 0;
        while (midline && !ss.eof()) {
            int val;
            char end;
            ss >> val;
            ss >> end;
            if (end == '\r' || end == '\n') midline = false;
            // cout << val << ",";
            
            for (int i = 0; i < fields.size(); i++) {
                TicketField* tf = fields[i];
                bool b = determination[posInLine][i];
                determination[posInLine][i] = (b && tf->isValid(val));
            }
            
            posInLine++;
        
        }
        
        // cout << endl;
    }
    
    // cout << errorRate << endl;
    
    // int ind = 0;
    // for (auto v : determination) {
    //     for (auto b : v) cout << b;
    //     cout << " ticketValue " << ind++ << endl;
    // }
    
    // brute force approach doesn't consider multiple possibilities in initial state
    // long departs = 1;
    // for (int i = 0; i < fields.size(); i++) {
    //     for (int j = 0; j < 6; j++) {
    //         if (determination[i][j]) {
    //             departs *= myTicket[i];
    //             // cout << "times " << myTicket[i] << endl;
    //         }
    //     }
    // }
    
    // part 2-2: boolean satisfiability
    vector<int> fieldOrder;
    for (auto v : determination) {
        fieldOrder.push_back(-1);
    }
    sat(&determination, &fieldOrder);
    
    long departs = 1;
    for (int i = 0; i < fieldOrder.size(); i++) {
        if (fieldOrder[i] < 6) {
            departs *= myTicket[i];
            // cout << "times " << myTicket[i] << endl;
        }
    }
    
    cout << departs << endl;

    return 0;
}