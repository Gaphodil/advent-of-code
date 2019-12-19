#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// k and x keys

// input: one big string of numbers (list of single digits)
// repeated phases - each phase constructs a new list of same length
// next phase determined by sum(a[i] * pattern_i[i]) % 10 = b[j]
// pattern is dependent on which OUTPUT element is being calculated
//    base pattern: 0, 1, 0, -1 ...
//    repeat each value by position in output list (ie. i+1)
//    then: skip the first value exactly one time
//      eg. pattern_3 = 0,0,1,1,1,0,0,0,-1,-1,-1,0,0,0...
// part one obj: after 100 phases, get first 8 digits
// oh wow did i misunderstand some of that

// part two: real input = input repeated 10000 times (oh no)
// first 7 digits of initial input == message offset
// message offset == location of 8-digit message in FINAL output
//    ie. # of digits to skip before reading 8 digits
// so. 65 * 10e6 huh - and e_ponential too?! heck
// optimizations:
//    formula for a given "pattern" for list element i:
//      pattern[((p+1) / i) % 4] * a[i]
//    there should only be 10*3 possibilities for ANY subcalc of b[j]
//      memoization can help? map of ((p+1)/i)%4)*10 + a[i] doubles up on 0 though
//        easy fi_ though, just hardcode 0/2 -> 0, bypass map so 10*2
//        can also ignore a[i] == 0 so map 9*2
//    this doesnt help with n^2 summation though, what's missing?
//    10000 / 650 = 15 R 250

// debug funcs

void print_vector(const vector<int>& v) {
  vector<int>::const_iterator it;
  for (it = v.begin(); it != v.end(); ++it) {
    cout << *it;
  }
  cout << endl;
}

// part one

int* generatePattern( const int phase, const int len) {
  int pattern[4] = {0,1,0,-1};
  int* arr = new int[len+1];
  int i = 0;
  int p = 0;
  while (i < len) {
    for (int j = 0; j < phase; j++) {
      arr[i] = pattern[p%4];
      i++;
      if (i >= len) break;
    }
    p++;
  }
  return arr;
}

int main() {
  char c;
  ifstream ifs("input.txt");
  vector<int> a;
  vector<int> b;
  int len = 0;
  
  while (ifs.get(c)) {
    if (c == '\n')
      break;
    a.push_back(atoi(&c));
    ++len;
  }
  
  ifs.close();
  
  // part two setup
  int offset = 0;
  for (int i = 0; i < 7; i++) {
    offset += a[i];
    offset = offset * 10;
  }
  offset = offset / 10;
  
  // sigh
  for (int i = 0; i < 10000-1; i++) {
    for (int j = 0; j < len; j++) {
      a.push_back(j);
    }
  }
  
  len = len * 10000;
  
  // cout << "input value: ";
  // print_vector(a);
  
  for (int i = 1; i <= 100; i++) {
    for (int j = 0; j < len; j++) {
      // does cpp have matri_ multiplication? 
      // hoo boy is this gonna be slow without it!!!!!
      // int* full_pattern = generatePattern(j+1, len+1);
      int temp = 0;
      for (int k = 0; k < len; k++) {
        temp += a[k] * full_pattern[k+1];
        // cout << a[k] << "*" << full_pattern[k+1] << "\t+ ";
      }
      b.push_back(abs(temp) % 10);
      // cout << "\t= " << b.back() << endl;
      delete[] full_pattern;
    }
    a = b;
    b.clear();
    
    // cout << "phase " << i << endl;
    // cout << "After phase " << i << ":";
    // print_vector(a);
  }
  
  // part two output
  for (int i = 0; i < 8; i++) {
    cout << a[offset+i];
  }
  cout << endl;
  
  return 0;
}

