#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cassert>
#include <unordered_map>

//  wasted a ton of time not having tested the new compiler beforehand
//  still not sure how iterators work
//  original thought: needed map of sets - then just orbit counts
//    but that's one way only, so just make a dang tree actually?
//  hour in, not a lot of progress, and busy otherwise; 
//    putting this off 'til tomorrow

//  okay it's tomorrow let's get this bread
//  okay i couldn't get this bread
//    reverse engineering someone else's c++ pt2 solution to learn something, maybe?

using namespace std;

typedef pair<string, string> orbitpair;
typedef unordered_map<string, vector<string>> Tree;

void printOrbitpairs(vector<orbitpair> orbit_pairs) {
  int len = orbit_pairs.size();
  for (int i = 0; i < len; i++){
    cout  << get<0>(orbit_pairs[i]) << " is orbited by " \
          << get<1>(orbit_pairs[i]) << endl;
  }
  cout << endl;
}

// part one
int orbitsCount(const Tree& t, string node, int depth) {
  Tree::const_iterator orb_iter; // heh
  int orbits = 0;
  cout << node << " has depth:\t" << depth << endl;
  orb_iter = t.find(node);
  if (orb_iter == t.end()) {
    return depth;
  }
  // eg. COM -> B -> G -> H(not in keys)
  //    main <- 6 <- 5 <- 3
  
  for (int i = 0; i < orb_iter->second.size(); i++) {
    orbits += orbitsCount(t, orb_iter->second[i], depth + 1);
  }
  orbits += depth;
  cout << "returning"+node+"with total orbits:\t" << orbits << endl;
  return orbits;
}

// part two (this is copied wholesale from a user on github)
int dfs(const Tree& t, string node, int depth) {
  if (node == "YOU" || node == "SAN") {
    cout << node << " found at depth:\t" << depth << endl;
    return depth;
  }
  
  Tree::const_iterator orb_iter; // heh
  orb_iter = t.find(node);
  if (orb_iter == t.end()) {
    return 0;
  }
  int first_depth = 0;
  for (int i = 0; i < orb_iter->second.size(); i++) {
    int next_depth = dfs(t, orb_iter->second[i], depth + 1);
    if (next_depth != 0) {
      if (first_depth != 0) {
        return first_depth + next_depth - 2*depth - 2;
      }
      first_depth = next_depth;
    }
  }
  return first_depth;
}

// "COM" is root (i missed this for a while)
int main() {
  vector<orbitpair> orbit_pairs;
  Tree orbiters;
  
  while (1){
    string temp, a, b;
    stringstream ss;
    getline(cin, temp);
    //cout << temp;
    if (cin.eof())
      break;
    
    ss << temp;
    getline(ss, a, ')');
    getline(ss, b);
    //cout <<a<<b<<endl;
    assert(ss.eof());
    orbit_pairs.push_back(make_pair(a,b));
    
    orbiters[a].push_back(b);
    
  }
  printOrbitpairs(orbit_pairs);
  
  string root = "COM";
  int total_orbits = orbitsCount(orbiters, root, 0);
  cout << total_orbits << endl;
  int you2san = dfs(orbiters, root, 0);
  cout << you2san << endl;
  
  return 0;
}

