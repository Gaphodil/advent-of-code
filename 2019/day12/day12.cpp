#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <cstdlib>

using namespace std;

// starting 12 min late heck yea! k and x keys still wack

// input = position of each moon
// obj: simulate motion
// 3-dimensional space
// initial velocity = 0
// environment: episodic (time steps)
// steps to take per time step:
//    1: update velocities based on gravity
//    2: update positions based on velocity
//  updating gravity: 
//    for each pair of moons, apply +1/-1 per axis difference (+-0 if same position on axis)
//  updating velocity:
//    add velocity to position
// next obj: calc total energy
//    = potential energy * kinetic energy
//    = \sum(abs(pos_coords)) * \sum(abs(vel_coords))
// output: total energy for system (all moons) after 1000 steps

// part one
typedef tuple<int, int, int> coords;

// debug
void printCoords(const coords& c) {
  cout << "("<<get<0>(c)<<", "<<get<1>(c)<<", "<<get<2>(c)<<")";
}

int calcEnergy(const coords& c) {
  int i,j,k,tot;
  i = abs(get<0>(c));
  j = abs(get<1>(c));
  k = abs(get<2>(c));
  tot = i+j+k;
  cout << i <<"+"<<j<<"+"<<k<<"="<<tot;
  return tot;
}

void printMoonStats(const vector<coords>& pos, const vector<coords>& vel) {
  int len = pos.size();
  
  for (int i = 0; i < len; i++) {
    cout << "pos=";
    printCoords(pos[i]);
    cout << "\tvel=";
    printCoords(vel[i]);
    cout << endl;
  }
}

void printEnergy(const vector<coords>& pos, const vector<coords>& vel) {
  int len = pos.size();
  int pot,kin,tot;
  int tot_all = 0;
  
  for (int i = 0; i < len; i++) {
    cout << "pot=";
    pot = calcEnergy(pos[i]);
    cout << "\tkin=";
    kin = calcEnergy(vel[i]);
    cout << "\ttot=";
    tot = pot*kin;
    tot_all += tot;
    cout << tot;
    cout << endl;
  }
  cout << "Grand total: " << tot_all << endl;
}

// do gravity calculations
void updateVel(const vector<coords>& pos, vector<coords>& vel) {
  int len = pos.size();
  coords moon1, moon2;
  int temp1, temp2;
  
  for (int i = 0; i < len-1; i++) {
    moon1 = pos[i];
    
    for (int j = i+1; j < len; j++) {
      moon2 = pos[j];
      
      temp1 = get<0>(moon1);
      temp2 = get<0>(moon2);
      
      if (temp1 < temp2) {
        get<0>(vel[i]) += 1;
        get<0>(vel[j]) -= 1;
      }
      else if (temp1 > temp2) {
        get<0>(vel[i]) -= 1;
        get<0>(vel[j]) += 1;
      }
      else ;
      
      temp1 = get<1>(moon1);
      temp2 = get<1>(moon2);
      
      if (temp1 < temp2) {
        get<1>(vel[i]) += 1;
        get<1>(vel[j]) -= 1;
      }
      else if (temp1 > temp2) {
        get<1>(vel[i]) -= 1;
        get<1>(vel[j]) += 1;
      }
      else ;
      
      temp1 = get<2>(moon1);
      temp2 = get<2>(moon2);
      
      if (temp1 < temp2) {
        get<2>(vel[i]) += 1;
        get<2>(vel[j]) -= 1;
      }
      else if (temp1 > temp2) {
        get<2>(vel[i]) -= 1;
        get<2>(vel[j]) += 1;
      }
      else ;
      
    }
  }
}

void updatePos(vector<coords>& pos, const vector<coords>& vel) {
  int len = pos.size();
  
  for (int i = 0; i < len; i++) {
    get<0>(pos[i]) += get<0>(vel[i]);
    get<1>(pos[i]) += get<1>(vel[i]);
    get<2>(pos[i]) += get<2>(vel[i]);
  }
}

// part two
int checkMatch(const vector<coords>& pos, const vector<coords>& state) {
  if (pos[0] == state[0]) {
    if (pos[1] == state[1]) {
      if (pos[2] == state[2]) {
        if (pos[3] == state[3]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

int main() {
  vector<coords> moons;
  vector<coords> moon_vels;
  string inp;
  int num_moons = 4;
  int x, y, z;
  
  for (int i = 0; i < num_moons; i++) {
    getline(cin, inp);
    stringstream isstream(inp);
    
    isstream.ignore(10, '=');
    isstream >> x;
    isstream.ignore(10, '=');
    isstream >> y;
    isstream.ignore(10, '=');
    isstream >> z;
    
    cout << x <<" "<< y <<" "<< z << endl;
    coords temp = make_tuple(x, y, z);
    moons.push_back(temp);
    
    temp = make_tuple(0,0,0);
    moon_vels.push_back(temp);
  }
  
  cout << "Timestep: 0" << endl;
  printMoonStats(moons,moon_vels);
  // part one
  for (int i = 1; i <= 1000; i++) {
 
  // part two - is it guaranteed that the repeated state must be the first? i don't think so
  // how to store a state efficiently? 24 numbers to remember, 12 if ignore velocity of state0
  // i may not know how to do this, actually
  // vector<coords> state0;
  // for (int i = 0; i < num_moons; i++) {
  //   state0.push_back(moons[i]);
  // }
  
  // int step = 0;
  // while (1) {
    updateVel(moons,moon_vels);
    updatePos(moons,moon_vels);
    // step++;
    if (step%10 == 0) {
      cout << "Timestep: "<< step << endl;
      printMoonStats(moons,moon_vels);
      cout << endl;
    }
    // if (checkMatch(moons,state0) == 0) {
    //   cout << "term at step " << step << endl;
    //   break;
    // }
  }
  
  // part one
  printEnergy(moons,moon_vels);
  
  return 0;
}

