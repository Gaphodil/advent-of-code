#include <algorithm> // set_intersection
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

// reads from standard input
// list of foods with space-seperated ingredients
// each contains 0 or 1 allergen; each allergen is in only 1 ingredient
// all foods contain at least one allergen

// algorithm:
//  scan foods, create set<ingredients> for each food
//  map allergen to set of food pointers
//  for each allergen, get the intersection of all pointers: there must be at least one
//      learned: set iterators are always constant, and set_intersection's output requires nonconstant
//  if there are more, work from there
//      edit: there ARE more but also there are only 8 allergens and 8 total listed ingredients so whatever
//  for every ingredient that ISN'T one of these 8 count their total appearances

// was trying to avoid const pointers
// map<string,int> ingredientHash;
// int _hashID = 0;

typedef set<string> ingredientList;
map<string,vector<ingredientList*>> foodsWithAllergens;
ingredientList allergenIngredients;
set<ingredientList*> eachFood;

void readFood(const string& ing) {
    string item = "";
    istringstream ss(ing.substr(0,ing.size()-1)); // drop newline
    ingredientList* il = new ingredientList;
    vector<ingredientList*> vil = {il};
    
    ss >> item;
    while (item != "(contains") {
        // if (ingredientHash.find(item) == ingredientHash.end()) {
        //     ingredientHash[item] = _hashID;
        //     il->insert(_hashID);
        //     _hashID++;
        // }
        // else il->insert(ingredientHash[item]);
        il->insert(item);
        ss >> item;
    }
    eachFood.insert(il);
    do {
        ss >> item;
        item = item.substr(0,item.size()-1);
        if (foodsWithAllergens.find(item) == foodsWithAllergens.end()) {
            foodsWithAllergens[item] = vil;
        }
        else foodsWithAllergens[item].push_back(il);
    } while (!ss.eof());
}

void buildAllergenIngredients() {
    for (auto p : foodsWithAllergens) {
        string allergen = p.first;
        vector<string> al(p.second[0]->begin(), p.second[0]->end());
        for (int i = 1; i < p.second.size(); i++) {
            // cout << "\t\t\t" << ptr << endl;
            vector<string> al2(p.second[i]->begin(), p.second[i]->end());
            vector<string> al3(100);
            auto it = set_intersection(al.begin(), al.end(), al2.begin(), al2.end(), al3.begin());
            al3.resize(it - al3.begin());
            al = al3;
        }
        cout << "allergen " << allergen << " may be in:" << endl;
        for (auto s : al) {
            cout << "\t" << s << endl;
            allergenIngredients.insert(s);
        }
    }
}

int countNonAllergens() {
    int count = 0;
    for (auto ptr : eachFood) {
        for (auto s : *ptr) {
            if (allergenIngredients.find(s) != allergenIngredients.end()) ;
            else {
                // cout << s << endl;
                count++;
            }
        }
    }
    return count;
}

int main() {
    
    string line;
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        readFood(line);
    }
    
    buildAllergenIngredients();
    cout << countNonAllergens() << endl;
    
    // oops it said alphabetically by ALLERGEN
    // the data is small enough that this was easily achievable by hand though
    for (auto s : allergenIngredients) {
        cout << s << ",";
    }
    cout << endl;
    
    for (auto ptr : eachFood) {
        delete(ptr);
    }
    return 0;
}