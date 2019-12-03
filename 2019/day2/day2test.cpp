#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  vector<int> ints;
  int input;
  while ((cin >> input) && (cin.ignore())) {
    ints.push_back(input);
  }
  for (int i=0; i < ints.size(); i++) {
    cout << ints[i] << ",";
  }
  cout << endl;
  return 0;
}