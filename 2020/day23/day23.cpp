#include <algorithm> // find
// #include <list>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// reads from standard input
// input is labelled clockwise
// first cup = current
// then 100 moves:
//  pick up 3 subsequent cups - remove from circle
//  select a destination with label = curr-1
//    if this would be a removed cup, subtract another 1
//    if this ever goes below 1 instead wrap around to 9
//  place 3 picked up cups immediately following destination, in exact same order
//  current cup = next in order

// part 2: absolutely no idea where to start, solution megathread greatly suggests linked-list
// probably a way to do with c++ list object but wasn't sure
// also only needed to be singly linked

const int _TURNS = 100;
const int _TURNS2 = 10000000;

int doMove(vector<int>& labels, int curpos) {
    int currcup = labels[curpos];
    vector<int> pickup;
    vector<int> remain;
    auto begit = labels.begin()+curpos;
    if (curpos < 6) {
        vector<int> temp(begit+1,begit+4);
        pickup = temp;
        remain = labels;
        begit = remain.begin()+curpos;
        remain.erase(begit+1,begit+4);
    }
    else { // forgot to keep track of order of pickup
        vector<int> temp(begit-5,begit+1);
        remain = temp;
        if (begit == labels.end()-1) {
            pickup = labels;
            begit = pickup.begin()+curpos;
            pickup.erase(begit-5,begit+1);
        }
        else {
            vector<int> temp2(begit+1,labels.end());
            temp2.insert(temp2.end(),labels.begin(),labels.begin()+(3-temp2.size()));
            pickup = temp2;
        }
    }
    
    // debug
    // cout << "currcup = " << currcup << endl;
    // cout << "labels = ";
    // for (auto in : labels)
    //     cout << in;
    // cout << endl;
    // cout << "pickup = ";
    // for (auto in : pickup)
    //     cout << in;
    // cout << endl;
    // cout << "remain = ";
    // for (auto in : remain)
    //     cout << in;
    // cout << endl;
    
    int tofind = currcup;
    int iters = 0;
    do {
        iters++;
        tofind = (tofind-1)%9;
        if (tofind == 0) tofind = 9;
        begit = find(remain.begin(), remain.end(), tofind);
        if (begit != remain.end()) break;
        if (iters >= 7) {
            for (auto in : remain) {
                cout << in;
            }
            cout << endl << tofind << endl;
            throw runtime_error("inf loop");
        }
    } while (true);
    
    if (begit == remain.end()-1) {
        remain.insert(remain.begin(),pickup.begin(), pickup.end());
    }
    else remain.insert(begit+1,pickup.begin(), pickup.end());
    labels = remain;
    
    // cout << "labels = ";
    // for (auto in : labels)
    //     cout << in;
    // cout << endl;
    
    int newpos = 0;
    for (int i = 0; i < labels.size(); i++) {
        if (labels[i] == currcup) {
            newpos = (i+1)%9;
            // cout << "newpos = " << newpos << endl << endl;
            return newpos;
        }
    }
    throw runtime_error("value missing from labels");
}

class ListNode {
public:
    ListNode* Next;
    ListNode* Prev;
    int Val;
    ListNode(int val, ListNode* next, ListNode* prev) : Val(val), Next(next), Prev(prev) {}
};

int doBigMove(const map<int,ListNode*> &dict, int currcup) {
    ListNode* currnode = dict.at(currcup);
    ListNode* pickup = currnode->Next;
    currnode->Next = pickup->Next->Next->Next;
    
    int tofind = currcup;
    do {
        tofind = (tofind-1)%1000000;
        if (tofind == 0) tofind = 1000000;
        if (tofind == pickup->Val || tofind == pickup->Next->Val || tofind == pickup->Next->Next->Val);
        else break;
    } while (true);
    
    ListNode* dest = dict.at(tofind);
    pickup->Next->Next->Next = dest->Next;
    dest->Next = pickup;
    
    return currnode->Next->Val;
}

int main() {
    // vector<int> input = {1,9,8,7,5,3,4,6,2};
    vector<int> input = {3,8,9,1,2,5,4,6,7};
    
    // part 1
    // int curpos = 0;
    // for (int i = 0; i < _TURNS; i++) {
    //     if (i == 10) {
    //         for (auto in : input) 
    //             cout << in;
    //         cout << endl;
    //         // break;
    //     }
    //     curpos = doMove(input, curpos);
    // }
    
    // for (auto in : input) 
    //     cout << in;
    // cout << endl;
    
    // part 2
    // build input
    map<int,ListNode*> dict;
    ListNode* newInput = new ListNode(input[0],NULL,NULL);
    dict[input[0]] = newInput;
    
    for (int i = 1; i < input.size(); i++) {
        newInput = new ListNode(input[i],NULL,dict[input[i]]);
        dict[input[i]] = newInput;
        dict[input[i-1]]->Next = newInput;
    }
    
    newInput = new ListNode(10,NULL,dict[input[8]]);
    dict[10] = newInput;
    dict[input[8]]->Next = newInput;
    for (int i = 11; i <= 1000000; i++) {
        newInput = new ListNode(i,NULL,dict[i-1]);
        dict[i] = newInput;
        dict[i-1]->Next = newInput;
    }
    dict[1000000]->Next = dict[input[0]];
    
    // do moves
    int currcup = input[0];
    for (int i = 0; i < _TURNS2; i++) {
        currcup = doBigMove(dict, currcup);
        if (i < 10) {
            ListNode* temp = dict[1000000-9];
            for (int j = 0; j < 20; j++) {
                cout << temp->Val << " ";
                temp = temp->Next;
            }
            cout << endl << endl;
        }
    }
    
    long long val1 = (long long) dict[1]->Next->Val;
    long long val2 = (long long) dict[1]->Next->Next->Val;
    long long out = val1 * val2;
    cout << out << endl;

    return 0;
}