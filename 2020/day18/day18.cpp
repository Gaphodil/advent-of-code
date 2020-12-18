#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

// reads from standard input
// input is all positive single digit numbers

long parseLine(const string& eq) {
    stack<char> opStack;
    stack<long> valStack;
    
    char temp;
    istringstream ss(eq);
    
    bool wasOp = false;
    long rhs;
    char op;
    long lhs;
    long tempval;
    
    while (ss >> temp && temp != EOF && temp != '\r' && temp != '\n') {
        // cout << temp << endl;
        switch (temp) {
            case ')':
                wasOp = false;
                                
                if (opStack.top() == '(') { // open bracket
                    // cout << "popopen" << endl;
                    opStack.pop();
                }
                if (opStack.empty() || opStack.top() == '(') {
                    continue; // eventually will be another close bracket
                }
                
                rhs = valStack.top();
                valStack.pop();
                
                op = opStack.top();
                opStack.pop();
                
                lhs = valStack.top();
                valStack.pop();
                
                if (op == '+') tempval = lhs + rhs;
                else if (op == '*') tempval = lhs * rhs;
                else throw runtime_error("bad pop");
                
                valStack.push(tempval);
                break;
            case '(':
                wasOp = false;
                opStack.push(temp);
                break;
            case '+':
            case '*':
                wasOp = true;
                opStack.push(temp);
                break;
            default: 
                if (wasOp) {
                    rhs = atol(&temp);
                    
                    op = opStack.top();
                    opStack.pop();
                    
                    lhs = valStack.top();
                    valStack.pop();
                    
                    if (op == '+') tempval = lhs + rhs;
                    else if (op == '*') tempval = lhs * rhs;
                    else throw runtime_error("bad pop");
                    
                    valStack.push(tempval);
                }
                else valStack.push(atoi(&temp));
                wasOp = false;
        }
    }
    
    if (valStack.top() < 0) throw runtime_error("long overflow");
    return valStack.top();
}

long long parseLine2(const string& eq) {
    stack<char> opStack;
    stack<long long> valStack;
    
    char temp;
    string s(eq);
    s.insert(s.begin(),'(');
    s.push_back(')');
    istringstream ss(s);
    
    bool wasOp = false;
    bool popped;
    long long rhs;
    char op;
    long long lhs;
    long long tempval;
    
    while (ss >> temp && temp != EOF && temp != '\r' && temp != '\n') {
        // cout << temp << endl;
        switch (temp) {
            case ')':
                wasOp = false;
                
                while (!opStack.empty() && opStack.top() != '(') {
                    rhs = valStack.top();
                    valStack.pop();
                    
                    op = opStack.top();
                    opStack.pop();
                    
                    lhs = valStack.top();
                    valStack.pop();
                    
                    if (op == '+') tempval = lhs + rhs;
                    else if (op == '*') tempval = lhs * rhs;
                    else throw runtime_error("bad pop");
                    
                    // cout << "\ttemp: " << tempval << endl;
                    valStack.push(tempval);
                }
                
                if (!opStack.empty() && opStack.top() == '(') { 
                    // cout << "popopen" << endl;
                    opStack.pop();
                
                    while (!opStack.empty() && opStack.top() == '+') {
                        rhs = valStack.top();
                        valStack.pop();
                        
                        op = opStack.top();
                        opStack.pop();
                        
                        lhs = valStack.top();
                        valStack.pop();
                        
                        if (op == '+') tempval = lhs + rhs;
                        else throw runtime_error("bad pop");
                        
                        // cout << "\ttemp: " << tempval << endl;
                        valStack.push(tempval);
                    }
                }
                
                break;
            case '(':
                wasOp = false;
                opStack.push(temp);
                break;
            case '+':
                wasOp = true;
                opStack.push(temp);
                break;
            case '*':
                wasOp = false;
                opStack.push(temp);
                break;
            default: 
                if (wasOp) {
                    rhs = atol(&temp);
                    
                    op = opStack.top();
                    opStack.pop();
                    
                    lhs = valStack.top();
                    valStack.pop();
                    
                    if (op == '+') tempval = lhs + rhs;
                    else throw runtime_error("bad pop");
                    
                    // cout << "\ttemp: " << tempval << endl;
                    valStack.push(tempval);
                }
                else valStack.push(atoi(&temp));
                wasOp = false;
        }
    }
    
    long long ans = valStack.top();
    if (ans < 0) throw runtime_error("long long overflow");
    
    valStack.pop();
    if (!valStack.empty()) throw runtime_error("calculation incomplete");
    
    // cout << ans << endl;
    return ans;
}

int main() {
    long long sum = 0;
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        // sum += parseLine(line);
        sum += parseLine2(line);
        if (sum < 0) throw runtime_error("long long overflow");
    }
    
    // cout << parseLine2("1 + 2 * 3 + 4 * 5 + 6\r") << endl;
    // cout << parseLine2("1 + (2 * 3) + (4 * (5 + 6))\n") << endl;
    // cout << parseLine2("2 * 3 + (4 * 5)") << endl;
    // cout << parseLine2("5 + (8 * 3 + 9 + 3 * 4 * 3)") << endl;
    // cout << parseLine2("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") << endl;
    // cout << parseLine2("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") << endl;
    // cout << parseLine2("1 + (2 * 3) * (4 * 5) + 6") << endl; // (1+6)*(20+6)=7*26=182
    // cout << parseLine2("1 + (1 * 2) + (1 * 3) + 1") << endl; // 1+2+3+1=7
    // cout << parseLine2("1 + ((1 * 3) * (1 * 2)) + 1") << endl; // 1+6+1=8
    // cout << parseLine2("1 + (1 * 2) * (1 * 2) + 1") << endl; // 3*3=9
    // cout << parseLine2("1 + (1 * 2 + 3) * (1 + 2 * 3) + 1") << endl; // (1+5)*(9+1)=60
    // cout << parseLine2("2*(2*(2*(2*(2*(2*(2*2))))))") << endl; // 2^8 = 256
    // cout << parseLine2("((((((2*2)*2)*2)*2)*2)*2)*2") << endl; 
    // cout << parseLine2("2+(2+(2+(2+(2+(2+(2+2))))))") << endl; // 2*8 = 16
    // cout << parseLine2("((((((2+2)+2)+2)+2)+2)+2)+2") << endl; 
    
    // used another person's solution to find this last bug
    // (2 * 4 * (25 + 6) * (2 + 8)) + 4 = 31*4*2*10 + 4 = 2484
    // cout << parseLine2("(2 * 4 * (9 + 8 + 8) + 6 * 2 + 8) + 4") << endl; // gave 2064
    
    cout << sum << endl;

    return 0;
}