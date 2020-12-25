#include <iostream>
#include <vector>

using namespace std;

// reads from standard input

// start with value 1
// then, for $loop times, do:
//      value = value * subject
//      value = value % 20201227
// $loop: unknown! different for card AND door
// handshake example w/ subject = 7:
//      card: 7 -> loops -> card public key
//      door: 7 -> loops -> door public key
// user input: both public keys
//      card: DOOR public key -> CARD loops -> encryption key
//      door: CARD public key -> DOOR loops -> same encryption key

// example: 5764801 -> some nonsense -> loop = 8, subject = 7
//      17807724 -> some nonsense -> loop = 11, subject = 7
// verify: 5764801 -> 11 loops == 17807724 -> 8 loops

// actual problem: find encryption key, unknown = loop count, subject number

// guess/check finds a lot of solution but never with the same subject...
// but do they follow some sort of pattern? 1462, 3151, 3514...
// 3172848 works with subject 11675 but wrong answer?

// the first subject number is always 7??? that wasn't clearly presented at ALL, jeez

long doSteps(long value, int subject) {
    return (value*subject)%20201227;
}

int main() {
    long key1 = 14082811;
    long key2 = 5249543;

    long test1 = 5764801;
    long test2 = 17807724;
    
    bool testing = false;
    if (testing) {
        key1 = test1;
        key2 = test2;
    }

    int loops1 = 0;
    int loops2 = 0;
    // int subject1 = -1;
    // int subject2 = -2;
    // // assume loops/subject all < 30 for now?
    // for (int i = 11675; i <= 1000000; i++) { // subject
    //     long val = 1;
    //     for (int j = 1; j <= 1000000; j++) { // loop size
    //         // cout << "trying subject " << i << " on " << j << "loops" << endl;
    //         val = doSteps(val,i);
    //         // cout << "\tval = " << val << endl;
            
    //         if (val == key1) {
    //             loops1 = j;
    //             subject1 = i;
    //             cout << "key1: " << j << " loops, " << i << " as subject" << endl;
    //         }
    //         if (val == key2) {
    //             loops2 = j;
    //             subject2 = i;
    //             cout << "key2: " << j << " loops, " << i << " as subject" << endl;
    //         }
    //         if (subject1 == subject2) break;
    //     }
    //     // cout << endl;
    //     if (subject1 == subject2) break;
    // }
    
    long val = 1;
    for (long j = 1; j <= 100000000; j++) { // loop size
        // cout << "trying subject " << i << " on " << j << "loops" << endl;
        val = doSteps(val,7);
        // cout << "\tval = " << val << endl;
        
        if (val == key1) {
            loops1 = j;
            cout << "key1: " << j << " loops" << endl;
        }
        if (val == key2) {
            loops2 = j;
            cout << "key2: " << j << " loops" << endl;
        }
        if (loops1 != 0 && loops2 != 0) break;
    }
    
    if (loops2 != 0) {
        long ans1 = 1;
        long ans2 = 1;
        cout << "\n\tkey1: " << key1 << " loops: " << loops2 << endl;
        for (int i = 0; i < loops2; i++) {
            ans1 = doSteps(ans1,key1);
        }
        cout << "\tkey2: " << key2 << " loops: " << loops1 << endl;
        for (int i = 0; i < loops1; i++) {
            ans2 = doSteps(ans2,key2);
        }
        cout << ans1 << endl;
        cout << ans2 << endl;
    }

    return 0;
}