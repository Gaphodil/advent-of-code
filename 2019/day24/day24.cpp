#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cmath>

using namespace std;

// k and x keys

// biodiversity rating = 2**index
// hint to store things as 25 bits?
// game of life var: 
//    # -> . unless only one # adjacent
//    . -> # if one OR two # adjacent
// part one: bio rating of first layout appearing twice
// guess i could've just stored bio ratings directly (probably easier)
// yeah bit operations aren't working (i may regret not sticking with this)

// part two: only one level in infinite recursions
// grid is middle of 5x5, which is middle of bigger 5x5, etc.
// adjacent tiles include all tiles of different levels (see img)
// middle tile begins treated as empty
// all other layers begin as empty
// output: after 10 steps of life, what is sum(bugs) in 11 layers about 0
//    (input = 0, layers -5 to 5 inclusive)

// so within 10 steps, no bugs can escape more than 5 layers away from 0
// (two steps brings them within a layer's edge)
// thus there are only 11 maps max

// debug funcs

void printMatrix(const vector<vector<char>>& m) {
  for (auto v : m) {
    for (auto e : v) {
      cout << e;
    }
    cout << endl;
  }
  cout << endl;
}

void printMegaMatrix(const vector<vector<vector<char>>>& mm) {
  // i reversed the sign of depth by accident but otherwise it works fine
  int depth = -5;
  for (auto m : mm) {
    cout << "Depth " << depth << ":" << endl;
    for (auto v : m) {
      for (auto e : v) {
        cout << e;
      }
      cout << endl;
    }
    depth += 1;
    cout << endl;
  }
}

// void printUSet(const unordered_set<unsigned>& uset) {
//   for (auto e : uset) {
//     cout << e << endl;
//   }
//   cout << endl;
// }

// part one

// void gameOfLife(vector<vector<char>>& m) {
//   int len_y = m.size();
//   int len_x = m[0].size();
//   vector<vector<char>> m_new(m);
//   vector<char> empty_line(7, '.');
//   for (int i = 0; i < len_y; i++) {
//     m_new[i].insert(m_new[i].begin(), '.'); //linear
//     m_new[i].push_back('.'); //const
//   }
//   m_new.insert(m_new.begin(), empty_line);
//   m_new.push_back(empty_line);
  
//   for (int y = 0; y < len_y; y++) {
//     int new_y = y+1;
//     for (int x = 0; x < len_x; x++) {
//       int new_x = x+1;
//       int adj_bugs = 0;
      
//       if (m_new[new_y-1][new_x] == '#') adj_bugs += 1;
//       if (m_new[new_y+1][new_x] == '#') adj_bugs += 1;
//       if (m_new[new_y][new_x-1] == '#') adj_bugs += 1;
//       if (m_new[new_y][new_x+1] == '#') adj_bugs += 1;
      
//       if (m[y][x] == '#') {
//         if (adj_bugs != 1)
//           m[y][x] = '.';
//       }
//       else if (m[y][x] == '.') {
//         if ((adj_bugs == 1) || (adj_bugs == 2))
//           m[y][x] = '#';
//       }
//       else {
//         cout << "improper area layout???" << endl;
//         return;
//       }
//     }
//   }
//   return;
// }

// unsigned bioRating(const vector<vector<char>>& m) {
//   unsigned sum = 0;
//   int exp = 0;
//   for (auto v : m) {
//     for (auto e : v) {
//       if (e == '#')
//         sum += pow(2, exp);
//       exp += 1;
//     }
//   }
//   return sum;
// }

// int findMatch(const unordered_set<unsigned>& ulset, unsigned ui) {
//   auto found = ulset.find(ui);
//   if (found != ulset.end()) 
//     return 1;
//   return 0;
// }

// part two

void gameOfMultiLife(vector<vector<vector<char>>>& mm) {
  int layers = mm.size();
  int len_y = mm[0].size();
  int len_x = mm[0][0].size();
  
  vector<char> null_line(7, '.');
  vector<char> confused_line(7, '?');
  vector<vector<char>> null_area(7, null_line);
  vector<vector<vector<char>>> mm_new(mm);
  
  for (int i = 0; i < layers; i++) {
    for (int y = 0; y < len_y; y++) {
      mm_new[i][y].insert(mm_new[i][y].begin(), '?'); //linear
      mm_new[i][y].push_back('?'); //const
    }
    mm_new[i].insert(mm_new[i].begin(), confused_line);
    mm_new[i].push_back(confused_line);
    mm_new[i][3][3] = '?';
  }
  // mm_new.insert(mm_new.begin(), null_area);
  // mm_new.push_back(null_area);
  
  // unspecial cases: (x,y) = (1,1),(1,3),(3,1),(3,3)
  // special cases: y = 0, 4 | x = 0, 4 -> outer layer adj. (4)
  // specialer cases: (x,y) = (2,1),(1,2),(3,2),(2,3) -> inner layer adj. (8)
  
  unordered_set<int> normal = {11, 13, 31, 33};
  unordered_set<int> adj_into = {21, 12, 32, 23};
  
  for (int l = 0; l < layers; l++) { // 11
    int new_layer = l; //+1;
    
    for (int y = 0; y < len_y; y++) { // 5
      int new_y = y+1;
      
      for (int x = 0; x < len_x; x++) { // 5
        int new_x = x+1;
        if (mm_new[new_layer][new_y][new_x] == '?') {
          mm[l][y][x] = '?';
          continue;
        }
        
        int adj_bugs = 0;
        
        char adj_0 = mm_new[new_layer][new_y-1][new_x];
        char adj_1 = mm_new[new_layer][new_y][new_x-1];
        char adj_2 = mm_new[new_layer][new_y][new_x+1];
        char adj_3 = mm_new[new_layer][new_y+1][new_x];
        if (adj_0 == '#') adj_bugs += 1;
        if (adj_1 == '#') adj_bugs += 1;
        if (adj_2 == '#') adj_bugs += 1;
        if (adj_3 == '#') adj_bugs += 1;
        
        int additional = 0;
        int set_id = 10*x + y;
        if (normal.find(set_id) != normal.end());
        else if (adj_into.find(set_id) != adj_into.end()) {
          if (l == 0);
          else {
            int in_y, in_x;
            if (adj_0 == '?'){
              in_y = 4+1;
              for (int i = 0; i < 5; i++) {
                if (mm_new[new_layer-1][in_y][i+1] == '#')
                  additional += 1;
              }
            }
            else if (adj_1 == '?'){
              in_x = 4+1;
              for (int i = 0; i < 5; i++) {
                if (mm_new[new_layer-1][i+1][in_x] == '#')
                  additional += 1;
              }
            }
            else if (adj_2 == '?'){
              in_x = 0+1;
              for (int i = 0; i < 5; i++) {
                if (mm_new[new_layer-1][i+1][in_x] == '#')
                  additional += 1;
              }
            }
            else if (adj_3 == '?'){
              in_y = 0+1;
              for (int i = 0; i < 5; i++) {
                if (mm_new[new_layer-1][in_y][i+1] == '#')
                  additional += 1;
              }
            }
            else
              cout << "improper adj_into tiles" << endl;
          }
        }
        else {
          if (l == layers-1);
          else {
            if (adj_0 == '?')
              if (mm_new[new_layer+1][3-1][3] == '#') additional += 1;
            if (adj_1 == '?')
              if (mm_new[new_layer+1][3][3-1] == '#') additional += 1;
            if (adj_2 == '?')
              if (mm_new[new_layer+1][3][3+1] == '#') additional += 1;
            if (adj_3 == '?')
              if (mm_new[new_layer+1][3+1][3] == '#') additional += 1;
          }
        }
        
        adj_bugs += additional;
        if (mm[l][y][x] == '#') {
          if (adj_bugs != 1)
            mm[l][y][x] = '.';
        }
        else if (mm[l][y][x] == '.') {
          if ((adj_bugs == 1) || (adj_bugs == 2))
            mm[l][y][x] = '#';
        }
        else {
          cout << "improper area layout???" << endl;
          return;
        }
      }
    }
  }
  return;
}

unsigned countBugs(const vector<vector<vector<char>>>& mm) {
  unsigned count = 0;
  for (auto m : mm) {
    for (auto v : m) {
      for (auto e : v) {
        if (e == '#')
          ++count;
      }
    }
  }
  return count;
}

int main() {
  int side_len = 5;
  
  char c;
  ifstream ifs("input.txt");
  vector<vector<char>> area;
  unordered_set<unsigned> ratings;
  vector<char> line;
  
  while (ifs.get(c)) {
    if ((c == '\n') && (line.size() > 0)) {
      area.push_back(line);
      line.clear();
    }
    else {
      line.push_back(c);
    }
  }
  
  ifs.close();
  
  // part two
  int steps = 200;
  vector<vector<vector<char>>> rec_grid;
  vector<char> null_line(5, '.');
  vector<vector<char>> null_area(5, null_line);
  for (int i = 0; i < steps+1; i++) {
    if (i == steps/2)
      rec_grid.push_back(area);
    else
      rec_grid.push_back(null_area);
  }
  
  // printMegaMatrix(rec_grid);
  // cout << endl;
  for (int i = 1; i <= steps; i++) {
    gameOfMultiLife(rec_grid);
    // cout << "After " << i << " minute(s):" << endl;
    // printMegaMatrix(rec_grid);
    // cout << endl;
  }
  
  cout << countBugs(rec_grid) << endl;
  
  // printMatrix(area);
  // for (int i = 0; i < 4; i++) { // running on test input
  //   gameOfLife(area);
  //   cout << "After " << i+1 << " minute(s):" << endl;
  //   printMatrix(area);
  // }
  
  // part one
  /*
  unsigned ui = bioRating(area);
  ratings.insert(ui);
  while (1) {
    gameOfLife(area);
    ui = bioRating(area);
    if (findMatch(ratings, ui))
      break;
    ratings.insert(ui);
  }
  // printMatrix(area);
  // printUSet(ratings);
  
  cout << ui << endl;
  */
  
  return 0;
}

