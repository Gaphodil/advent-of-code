#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

// k and x keys

// shuffling deck of cards
// 10007 cards (0-indexed)
// begins factory ordered (0 top, 10006 bottom)
// 3 different techniques!
//    "deal into new stack": 
//      deal top card of deck to top of new stack until out (reversal)
//      use as new deck
//    "cut N":
//      take top N from top and move to bottom (subsection)
//      if N < 0 move |N| from bottom and put to top
//    "deal with increment N":
//      oh no
//      is this only linear? probably can be optimized but idk how (again)
//      pos[0] -> [0], pos[1] -> [N%10007], pos[2] -> [2N%10007], etc.

// part two: 
//    decksize =         119315717514047 (long long)
//    repeat the process 101741582076661 times in a row
// well, f:A -> B of the process once is independant of what A's values are
// how to track the translation? then how to implement it?

typedef enum technique {restack, cut, increment} tech;

// debug funcs

void printDeck(const vector<int>& v) {
  if (v.size() > 10)
    return;
  for (auto e : v) {
    cout << e << ' ';
  }
  cout << endl;
}

// part one

void restackDeck(vector<int>& v) {
  reverse(begin(v), end(v));
}

void cutDeck(vector<int>& v, int n) {
  if (n == 0) return;
  // 2*O(size(v))
  vector<int> temp;
  vector<int>::iterator it;
  if (n < 0) 
    it = end(v)-abs(n);
  else 
    it = begin(v)+n;
  temp.insert(end(temp), it, end(v));
  temp.insert(end(temp), begin(v), it);
  v = temp;
}

void incrementDeck(vector<int>& v, int n) {
  if (n <= 1) return;
  // 2*O(size(v)
  int len = v.size();
  vector<int> temp(len);
  for (int i = 0; i < len; i++) {
    // supposedly input is well formed
    temp[(i*n)%len] = v[i];
  }
  v = temp;
}

int main() {
  // int decksize = 10007; // part one
  long long decksize = 119315717514047; // part two
  
  string s;
  ifstream ifs("input.txt");
  vector<int> deck;
  vector<tech> acts;
  vector<int> args;
  int len = 0;
  
  //int i;
  long long i;
  
  for (i = 0; i < decksize; i++) {
    deck.push_back(i);
  }
  
  while (1) {
    getline(ifs ,s);
    if (s == "") break;
    
    ++len;
    stringstream ss(s);
    string word;
    int arg;
    
    ss >> word;
    if (word == "cut") {
      ss >> arg;
      acts.push_back(cut);
      args.push_back(arg);
    }
    else if (word == "deal") {
      ss >> word;
      if (word == "with") {
        ss >> word >> arg;
        acts.push_back(increment);
        args.push_back(arg);
      }
      else if (word == "into") {
        acts.push_back(restack);
        args.push_back(0);
      }
      else 
        cout << "input error: 1" << endl;
    }
    else 
      cout << "input error: 0" << endl;
  }
  
  ifs.close();
  
  for (i = 0; i < len; i++) {
    tech action = acts[i];
    int arg = args[i];
    
    if (action == cut)
      cutDeck(deck, arg);
    else if (action == restack)
      restackDeck(deck);
    else if (action == increment)
      incrementDeck(deck, arg);
  }
  // after one runthrough: ~200*decksize operations -> 2 quintillion
  // nah i don't know enough for this
  
  // printDeck(deck);
  
  // if (decksize == 10007) {
  //   for (i = 0; i < decksize; i++) {
  //     if (deck[i] == 2019) {
  //       cout << i << endl;
  //       break;
  //     }
  //   }
  // }
  
  cout << deck[2020] << endl;
  
  return 0;
}

