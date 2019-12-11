#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cmath>

using namespace std;

//  .   = empty
//  #   = asteroid (smaller than on map, e_act center (my _ key is broken, x.x))
//  (0,0) = top left
//  obj: monitoring station - detect any # w/ line of sight (any angle!)
//    must be on an asteroid
//  check: unique slopes?
//  part two: hahahaha you didn't use raytracing? try again bucko

// part one
int lineOfSight(const int& ast_x, const int& ast_y, const vector<string>& map) {
  int rows = map.size();
  int cols = map[0].size();
  unordered_set<float> pos_slopes; // clockwise from straight up
  unordered_set<float> neg_slopes; // clockwise from straight down
  //  slope from a point:
  //    top right     -> rise   / run   = +float
  //    bottom right  -> -rise  / run   = -float
  //    bottom left   -> -rise  / -run  = +float
  //    top left      -> rise   / -run  = -float
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i==ast_y && j==ast_x){
        //cout << "skip (" <<j<<", "<<i<<")"<<endl; // my k and w keys are broken too
        continue;
      }
      
      if (map[i][j] == '#') {
        float slope = (float)(i-ast_y) / (float)(j-ast_x);
        //cout << "slope to ("<<j<<", "<<i<<") = " << slope << endl;
        if (j > ast_x || (i == ast_y && slope > 0.0)) { // right or straight up
          // an error in this calculation caused my answer to be off by one (p1) :<
          if (pos_slopes.find(slope) != pos_slopes.end()) ;
          else {
            pos_slopes.insert(slope);
            //cout << "\tadded " << slope << " to pos" << endl;
          }
        }
        else {  // left or straight down
          if (neg_slopes.find(slope) != neg_slopes.end()) ;
          else {
            neg_slopes.insert(slope);
            //cout << "\tadded " << slope << " to neg" << endl;
          }
        }
      }
      
    }
  }
  
  int unique_slopes = pos_slopes.size() + neg_slopes.size();
  return unique_slopes;
}

// part two
// looked up some hints, i never even considered using angles dang
// yeah i'll hold off 'til tomorrow again
void vapoRays(const int& ast_x, const int& ast_y, vector<string> map) {
  int cursor_x, cursor_y, destroyed_this_pass;
  int rows = map.size();
  int cols = map[0].size();
  
  // start pointing up (y->0) and clockwise (x++)
  while (1) {
    cursor_x = ast_x;
    cursor_y = ast_y;
    
    if (destroyed_this_pass == 0)
      break;
  }
  
}

// part one
int main() {
  string line;
  vector<string> space_map;
  int rows, cols;
  
  while (1) {
    getline(cin,line);
    if (line == "")
      break;
    space_map.push_back(line);
  }
  
  rows = space_map.size();
  cols = space_map[0].size();
  
  cout << rows <<"\t"<< cols << endl;
  
  int most_slopes = -1;
  int cur_slopes;
  int best_x;
  int best_y;
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (space_map[i][j] == '#') {
        cur_slopes = lineOfSight(j, i, space_map);
        cout  << "Asteroid (" << j << ", " << i \
              << ") has " << cur_slopes << " asteroids in sight" << endl;
        if (cur_slopes > most_slopes) {
          most_slopes = cur_slopes;
          best_y = i;
          best_x = j;
        }
        
      }
    }
  }
  
  cout << best_x << ", " << best_y << " sees " << most_slopes << endl << endl;
  
  // part two
  vapoRays(best_x, best_y, space_map);
  
  return 0;
}

