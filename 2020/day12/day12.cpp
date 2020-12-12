#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// reads from standard input

// typedef enum {north, east, south, west} compass;

void forward(const int dir, const int val, int* ns, int* ew) {
    switch (dir) {
        case 0: *ns += val; break;
        case 1: *ew += val; break;
        case 2: *ns -= val; break;
        case 3: *ew -= val; break;
        default: throw runtime_error("invalid facing");
    }
}

int main() {
    string line;
    
    // part 1
    // compass facing = east;
    // int facing = 1;
    // int ns_disp = 0;
    // int ew_disp = 0;
    
    // while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
    //     istringstream ss(line);
    //     char act;
    //     int val;
    //     ss >> act >> val;
        
    //     switch (act) {
    //         case 'N': ns_disp += val; break;
    //         case 'S': ns_disp -= val; break;
    //         case 'E': ew_disp += val; break;
    //         case 'W': ew_disp -= val; break;
    //         case 'L': facing = (facing - (val/90) + 4) % 4; break;
    //         case 'R': facing = (facing + (val/90)) % 4; break;
    //         case 'F': forward(facing, val, &ns_disp, &ew_disp); break;
    //         default: throw runtime_error("invalid action");
    //     }
        
    //     // cout << "coords:\t" << ns_disp << "\t" << ew_disp << endl;
    // }
    
    // part 2
    int facing = 1;
    int ns_disp = 0;
    int ew_disp = 0;
    int wp_ns = 1;
    int wp_ew = 10;
    
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        istringstream ss(line);
        char act;
        int val;
        ss >> act >> val;
        
        int temp;
        switch (act) {
            case 'N': wp_ns += val; break;
            case 'S': wp_ns -= val; break;
            case 'E': wp_ew += val; break;
            case 'W': wp_ew -= val; break;
            case 'L': for (int i = 0; i < val/90; i++) {
                temp = wp_ew;
                wp_ew = -wp_ns; 
                wp_ns = temp; 
            } break;
            case 'R': for (int i = 0; i < val/90; i++) {
                temp = wp_ew;
                wp_ew = wp_ns; 
                wp_ns = -temp; 
            } break;
            case 'F': ns_disp += val*wp_ns; ew_disp += val*wp_ew; break;
            default: throw runtime_error("invalid action");
        }
        
        // cout << "coords:\t" << ns_disp << "\t" << ew_disp << endl;
        // cout << "waypnt:\t" << wp_ns << "\t" << wp_ew << endl;
    }
    
    cout << abs(ns_disp) + abs(ew_disp) << endl;

    return 0;
}