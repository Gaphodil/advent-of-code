#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// reads from standard input
// decks listed top first - store as queue
// both draw top, larger player adds both to the bottom of their deck (largest first)
// repeat until one player has all cards
// default input is 25*2, test input is 5*2
// winning score is card * (decksize - queueposition)

// new rules:
// if a previous round (in current game) had the EXACT same cards in the same order in both decks
//      P1 wins instantly
// if both players (decklen (after draw) >= drawn card value) winner of round is now determined by a NEW GAME
// otherwise: at least one player must not have enough cards left and winner is higher card
//      clarification: adding cards to deck is WINNER first, then loser (not always w>l)
// to start a subgame, create a COPY of the NEXT (card value) cards

const int _DECKSIZE = 50;
// const int _DECKSIZE = 10;

int playGame(queue<int> p1, queue<int> p2) {
    while (!(p1.empty() || p2.empty())) {
        int c1 = p1.front();
        p1.pop();
        int c2 = p2.front();
        p2.pop();
        
        if (c1 > c2) {
            p1.push(c1);
            p1.push(c2);
        }
        else if (c2 > c1) {
            p2.push(c2);
            p2.push(c1);
        }
        else throw runtime_error("cards repeated when no duplicates");
    }
    
    queue<int> winner;
    if (p1.empty()) winner = p2;
    else winner = p1;
    
    int pos = 0;
    long score = 0;
    while (!winner.empty()) {
        int c = winner.front();
        winner.pop();
        score += c * (_DECKSIZE-pos);
        pos++;
    }
    
    return score;
}

int playRecursiveGame(vector<int> p1, vector<int> p2) {
    int decksize = p1.size() + p2.size();
    vector<vector<int>> layouts1;
    vector<vector<int>> layouts2;
    int winner = 0;
    
    while (!(p1.empty() || p2.empty())) {
        // before draw, consult previous orders
        for (int i = 0; i < layouts1.size(); i++) {
            if (layouts1[i] == p1 && layouts2[i] == p2) {
                winner = 1;
                break;
            }
        }
        if (winner != 0) break;
        // add to stored configurations for current game
        layouts1.push_back(p1);
        layouts2.push_back(p2);
        
        // do the draw - it's a new configuration
        int c1 = p1[0];
        p1.erase(p1.begin());
        int c2 = p2[0];
        p2.erase(p2.begin());
        
        // check if players have cards remaining >= card drawn and recurse if so
        if (p1.size() >= c1 && p2.size() >= c2) {
            vector<int> newp1(p1.begin(), p1.begin()+c1);
            vector<int> newp2(p2.begin(), p2.begin()+c2);
            int localwin = playRecursiveGame(newp1, newp2);
            if (localwin > 0) {
                p1.push_back(c1);
                p1.push_back(c2);
            }
            else if (localwin < 0) {
                p2.push_back(c2);
                p2.push_back(c1);
            }
            else throw runtime_error("recursive game returned 0 score");
        }
        else { // winner is the larger card
            if (c1 > c2) {
                p1.push_back(c1);
                p1.push_back(c2);
            }
            else if (c2 > c1) {
                p2.push_back(c2);
                p2.push_back(c1);
            }
            else throw runtime_error("cards repeated when no duplicates");
        }
    }
    
    vector<int>* winningDeck;
    if (winner == 0) winner = p1.empty() ? 2 : 1;
    winningDeck = (winner == 2) ? &p2 : &p1;
    
    long score = 0;
    for (int pos = 0; pos < winningDeck->size(); pos++) {
        int c = winningDeck->at(pos);
        score += c * (decksize-pos);
    }
    
    if (winner == 1) return score; // information about which player won
    return -score;
}

int main() {
    string line;
    
    // queue<int> p1;
    vector<int> p1;
    getline(cin,line);
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        // p1.push(stoi(line));
        p1.push_back(stoi(line));
    }
    
    // queue<int> p2;
    vector<int> p2;
    getline(cin,line);
    while (!getline(cin,line).eof() && line != "\n" && line != "\r") {
        // p2.push(stoi(line));
        p2.push_back(stoi(line));
    }
    
    // cout << playGame(p1,p2) << endl;
    cout << playRecursiveGame(p1,p2) << endl;

    return 0;
}