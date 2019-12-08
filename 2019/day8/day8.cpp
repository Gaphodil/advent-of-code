#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
typedef unordered_map<int,int> layer;

// segfaulted on one run but disappeared on recompile; not sure where

// part one
int main() {
  int dim_x = 25, dim_y = 6;
  int layer_size = dim_x*dim_y;
  char inp;
  int digit;
  vector<layer> layers;
  int layer_i = 0;
  int zero_count = dim_y*dim_x;
  int min_layer = -1;
  
  // part two: init
  int final_layer[layer_size];
  for (int i = 0; i < layer_size; i++) {
    final_layer[i] = 2;
  }
  
  cout << "layer size:\t" << layer_size << endl;
  
  while (1) {
    // process layer
    layer temp_layer;
    for (int i = 0; i < layer_size; i++) {
      cin.get(inp);
      if (inp == '\n')
        break;
      digit = atoi(&inp);
      temp_layer[digit]++;
      
      // part two
      if ((final_layer[i] == 2) && (digit != 2))
        final_layer[i] = digit;
    }
    if (inp == '\n')
      break;
    
    // debug
    layers.push_back(temp_layer);
    cout << "layer " << layer_i << " has:\t";
    for (int i = 0; i < 10; i++) {
      cout << layers[layer_i][i] << ' ';
    }
    cout << endl; 
    
    // count 0
    if (temp_layer[0] < zero_count) {
      zero_count = temp_layer[0];
      min_layer = layer_i;
    }
    
    layer_i++;
  }
  
  // count 1*2
  cout << "1x2 for layer " << min_layer <<":\t";
  cout << layers[min_layer][1] * layers[min_layer][2] << endl;
  
  // part two: show final_layer
  for (int y = 0; y < dim_y; y++) {
    for (int x = 0; x < dim_x; x++) {
      switch(final_layer[x+dim_x*y]) {
        case 0: 
          cout << '.';
          break;
        case 1:
          cout << 'X';
          break;
        case 2:
          cout << ' ';
      }
    }
    cout << endl;
  }

  return 0;
}

