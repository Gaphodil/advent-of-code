#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>

/*  On reflection, things to work on:
 *    Getting input - getline and sstream seems to work fine, although
 *      still not fully understood
 *    Understanding the problem faster - get pen and paper?
 *    Code's pretty messy - comment more clearly
 *    Figure out how specific datatypes work before using them incorrectly
 */

using namespace std;

//  part one:
//    construct a list of coordinates for one path,
//    then check for intersections on the other
//    assuming: no parallel wires (this assumption is wrong but the code works?)
typedef pair<int, int> intpair;
typedef pair<char, int> segment;

intpair traverseSegment(segment seg) {
  char dir = get<0>(seg);
  int dist = get<1>(seg);
  intpair vect = make_pair(0,0);
  switch (dir) {
  case 'U':
    get<1>(vect) += dist;
    break;
  case 'R':
    get<0>(vect) += dist;
    break;
  case 'D':
    get<1>(vect) -= dist;
    break;
  case 'L':
    get<0>(vect) -= dist;
    break;  
  }
  return vect;
}

void printCoords(vector<intpair> coords) {
  for (int i = 0; i < coords.size()-1; i++) {
    cout << '(' << get<0>(coords[i]) << ", " << get<1>(coords[i]) << "),";
  }
  cout << '(' << get<0>(coords[coords.size()-1]) << ", " << get<1>(coords[coords.size()-1]) << ")" << endl;
}

void printSegments(vector<segment> segments) {
  for (int i = 0; i < segments.size()-1; i++) {
    cout << '(' << get<0>(segments[i]) << ", " << get<1>(segments[i]) << "),";
  }
  cout << '(' << get<0>(segments[segments.size()-1]) << ", " << get<1>(segments[segments.size()-1]) << ")" << endl;
}


int main() {
  vector<segment> path1;
  vector<segment> path2;
  
  char direction;
  int distance;
  char extra;
  string str1;
  string str2;
  
  getline(cin, str1);
  getline(cin, str2);
  str1 = str1+".";
  str2 = str2+".";
  
  stringstream line1(str1);
  while((line1 >> direction >> distance) && (line1.ignore())) {
    path1.push_back(make_pair(direction, distance));
  }
  
  stringstream line2(str2);
  while ((line2 >> direction >> distance) && (line2.ignore())) {
    path2.push_back(make_pair(direction, distance));
  }
  
  intpair cur_coords = make_pair(0,0);
  vector<intpair> path1_coords;
  path1_coords.push_back(cur_coords);
  
  for (int i = 0; i < path1.size(); i++) {
    intpair diff = traverseSegment(path1[i]);
    path1_coords.push_back(make_pair(get<0>(cur_coords)+get<0>(diff), get<1>(cur_coords)+get<1>(diff)));
    cur_coords = path1_coords.back();
  }
  // coords.size() = path1.size() + 1
  
  // printSegments(path1);
  // printSegments(path2);
  // printCoords(path1_coords);
  
  cur_coords = make_pair(0,0);
  int min_dist = 9999999;
  int manhattan;
  // part two:
  int min_steps = 9999999;
  int cur_steps;
  int steps1;
  int steps2 = 0;
  
  for (int i = 0; i < path2.size(); i++) {
    intpair diff = traverseSegment(path2[i]);
    int cur0 = get<0>(cur_coords);
    int cur1 = get<1>(cur_coords);
    int next0 = cur0+get<0>(diff);
    int next1 = cur1+get<1>(diff);
    
    steps1 = 0;
    for (int j = 0; j < path1.size(); j++) {
      int a0 = get<0>(path1_coords[j]);
      int a1 = get<1>(path1_coords[j]);
      int b0 = get<0>(path1_coords[j+1]);
      int b1 = get<1>(path1_coords[j+1]);
      
      if (cur0 == next0){
        if (b0 == a0);
        // cur u-d, path1 l-r
        // cur0 == next0, a1 == b1
        // check: a0 < cur0 < b0 and cur1 < a1 < next1
        else if ((cur0 > min(a0, b0)) && (cur0 < max(a0, b0))) {
          if ((a1 > min(cur1, next1)) && (a1 < max(cur1, next1))) {
            manhattan = abs(cur0)+abs(a1);
            // steps for final section: from a0 to cur0 + from cur1 to a1
            cout << "cur_steps = " << steps1 << " + " << steps2 << " + " <<abs(cur0-a0)<< " + " <<abs(a1-cur1)<<endl;
            cur_steps = steps1 + steps2 + abs(cur0 - a0) + abs(a1 - cur1);
            
            cout << '(' << cur0 << ", " << a1 << ")\tdistance = " \
              << manhattan << "\tsteps = " << cur_steps << endl;
              
            if (manhattan < min_dist)
              min_dist = manhattan;
            if (cur_steps < min_steps)
              min_steps = cur_steps;
          }
        }
      }
      else if (cur1 == next1){
        if (b1 == a1);
        // cur l-r, path1 u-d
        // cur1 == next1, a0 == b0
        // check: a1 < cur1 < b1 and cur0 < a0 < next0
        else if ((cur1 > min(a1, b1)) && (cur1 < max(a1, b1))) {
          if ((a0 > min(cur0, next0)) && (a0 < max(cur0, next0))) {
            manhattan = abs(a0)+abs(cur1);
            cout << "cur_steps = " << steps1 << " + " << steps2 << " + " <<abs(cur0-a0)<< " + " <<abs(a1-cur1)<<endl;
            cur_steps = steps1 + steps2 + abs(a0 - cur0) + abs(cur1 - a1);
            
            cout << '(' << a0 << ", " << cur1 << ")\tdistance = " \
              << manhattan << "\tsteps = " << cur_steps << endl;
            
            if (manhattan < min_dist)
              min_dist = manhattan;
            if (cur_steps < min_steps)
              min_steps = cur_steps;
          }
        }
      }
      cout << "steps1 += " << abs(b0-a0) << " + " << abs(b1-a1); 
      steps1 += abs(b0-a0) + abs(b1-a1);
      cout << " = " << steps1 << endl;
    }
    cur_coords = make_pair(next0, next1);
    cout << "\tsteps2 += " << abs(get<0>(diff)) << " + " << abs(get<1>(diff));
    steps2 += abs(get<0>(diff)) + abs(get<1>(diff));
    cout << " = " << steps2 << endl;
    // debug
    // cout << '(' << get<0>(cur_coords) << ", " << get<1>(cur_coords) << ')' << endl;
  }
  
  cout << "min dist:\t" << min_dist << endl;
  cout << "min steps:\t" << min_steps << endl;
  
  return 0;
}