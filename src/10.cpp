#include "common.hpp"

vector<unsigned long> _f10(vector<string> input) {
    map<char, int> opening = {{'(', 1}, {'{', 3}, {'[', 2}, {'<', 4}};
    map<char, pair<char, int>> closing = {
        {')', pair{'(', 3}}, 
        {']', pair{'[', 57}}, 
        {'}', pair{'{', 1197}}, 
        {'>', pair{'<', 25137}}, 
    };
    unsigned long invalid = 0;
    vector<unsigned long> incompletes;
    for (string str: input) {
        deque<char> stack;
        int invalidScore = 0;
        for (char ch: str) {
            if (opening.count(ch)) {
                stack.push_front(ch);
            } else if (closing.count(ch)) {
                auto [expect, errorScore] = closing[ch];
                if (stack.front() != expect) {
                    invalidScore = errorScore;
                } else {
                    stack.pop_front();
                }
            }
            if (invalidScore) break;
        }
        unsigned long incompleteScore = 0;
        if (!invalidScore) {
            for (char ch: stack) {
                incompleteScore *= 5ul;
                incompleteScore += (unsigned long) opening[ch];
            }
            incompletes.push_back(incompleteScore);
        }
        //cout << str << ' ' << invalidScore << " " << incompleteScore << endl;
        invalid += invalidScore;
    }
    sort(incompletes.begin(), incompletes.end());
    // for( auto v: incompletes) cout << v << "\n"; cout << endl;
    unsigned long incomplete = incompletes[incompletes.size() / 2];
    return vector<unsigned long>{invalid, incomplete}; 
}

unsigned long f10(vector<string> input) {
    return _f10(input)[0];
}
unsigned long f10a(vector<string> input) {
    return _f10(input)[1];
}

int main() {
    cout << "10\t" << f10(getContent("./input/input-10.txt")) << endl;
    cout << "10a\t" << f10a(getContent("./input/input-10.txt")) << endl;
    return 0;
}
