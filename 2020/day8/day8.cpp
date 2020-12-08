#include <iostream>
#include <map>
#include <vector>

using namespace std;

// reads from standard input

typedef enum {acc, jmp, nop} op_t;

op_t getOp(string s) {
    if (s == "acc") return acc;
    if (s == "jmp") return jmp;
    if (s == "nop") return nop;
    throw runtime_error("invalid op string");
}

class Instruction {
public:
    op_t Op;
    int Arg;
    bool runBefore = false;
    Instruction(op_t op, int arg) : Op(op), Arg(arg) {}
    ~Instruction() {}
    void print() {cout << Op << " " << Arg << endl;}
};
typedef Instruction inst_t;

int part1(vector<inst_t*> program, int ind, int acc) {
    inst_t* inst = program[ind];
    if (inst->runBefore) return acc;
    inst->runBefore = true;
    
    switch(inst->Op) {
        case 0: return part1(program, ind+1, acc+inst->Arg);
        case 1: return part1(program, ind+inst->Arg, acc);
        case 2: return part1(program, ind+1, acc);
        default: cerr << "invalid op string" << endl; return -1;
    }
}

op_t flip(op_t i) {
    return i==jmp ? nop : jmp;
}

int part2helper(vector<inst_t*> program, int ind, int acc) {
    if (ind >= program.size()) return acc;
    
    inst_t* inst = program[ind];
    if (inst->runBefore) return -1;
    inst->runBefore = true;
    
    switch(inst->Op) {
        case 0: return part2helper(program, ind+1, acc+inst->Arg);
        case 1: return part2helper(program, ind+inst->Arg, acc);
        case 2: return part2helper(program, ind+1, acc);
        default: cerr << "invalid op string" << endl; return -1;
    }
}

int part2(vector<inst_t*> program) {
    for (auto inst : program) {
        if (inst->Op == 0) continue;
        inst->Op = flip(inst->Op);
        
        int acc = part2helper(program, 0, 0);
        if (acc != -1) return acc;
        for (auto inst2 : program) {
            inst2->runBefore = false;
        }
        inst->Op = flip(inst->Op);
        
    }
    return -1;
}

int main() {
    vector<inst_t*> program;
    string line;
    while (!getline(cin,line).eof() && line != "\r" && line != "\n") {
        op_t o = getOp(line.substr(0,3));
        int arg;
        if (line[4] == '+') arg = stoi(line.substr(5));
        else arg = stoi(line.substr(4));
        
        inst_t* inst = new inst_t(o, arg);
        program.push_back(inst);
        
        // inst->print();
    }
    
    // cout << part1(program, 0, 0) << endl;
    cout << part2(program) << endl;

    return 0;
}