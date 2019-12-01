#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

// reads from standard input
// e.g. ./day1_2.exe < input.txt

// part one
int fuel_per_module(int mass) {
  return mass / 3 - 2;
}

// part two
int fuel_recursive(int mass) {
  int massFuel = fuel_per_module(mass);
  if (massFuel < 9) 
    return massFuel;
  return massFuel + fuel_recursive(massFuel);
}

int main() {
  int total = 0;
  int x;
  while (cin >> x && x != EOF) {
    //total += fuel_per_module(x);  // part one
    total += fuel_recursive(x);   // part two
  }
  cout << total << endl;
  return 0;
}