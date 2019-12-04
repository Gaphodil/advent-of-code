#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

//  part one:
//    seperating digits is very difficult apparently????
//    try stringstream next time, also check mingw installation to fix to_string() bug
int main() {
  // input
  int min = 347312;
  int max = 805915;
  
  int total = 0;
  
  for (int i = min; i <= max; i++) {
    vector<int> digits;
    int temp = i;
    int digit;
    for (int j = 0; j < 6; j++) {
      digit = temp%10;
      temp = temp/10;
      digits.insert(digits.begin(),digit);
    }
      
    // cout << digits[0] << digits[1] << digits[2] \
      << digits[3] << digits[4] << digits[5] << endl;
    if ((digits[0] <= digits[1]) && 
        (digits[1] <= digits[2]) && 
        (digits[2] <= digits[3]) && 
        (digits[3] <= digits[4]) && 
        (digits[4] <= digits[5]) ) {
      // part one:
      /*if ((digits[0] == digits[1]) || 
          (digits[1] == digits[2]) || 
          (digits[2] == digits[3]) || 
          (digits[3] == digits[4]) || 
          (digits[4] == digits[5]) )
        total += 1; */
      
      // part two:
      int count[10];
      for (int j = 0; j < 10; j++)
        count[j] = 0;
      for (int j = 0; j < 6; j++) 
        count[digits[j]] += 1;
      
      int is_only_double = 0;
      for (int j = 0; j < 10; j++) {
        if (count[j] == 2) {
          is_only_double = 1;
          break;
        }
      }
      if (is_only_double)
        total++;
    }
  }
  cout << total << endl;
  return 0;
}