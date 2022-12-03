#include "common.hpp"

/*
012345678910
##B#C#B#D##
  0 1 2 3
  A B C D
*/
map<char, int> scores{{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};
map<int, vector<vector<pair<int, int>>>> moves{
    {0, {{{1, 2}, {0, 3}}, {{3, 2}, {5, 4}, {7, 6}, {9, 7}, {10, 8}}}},
    {1, {{{3, 2}, {1, 4}, {0, 5}}, {{5, 2}, {7, 4}, {9, 6}, {10, 7}}}},
    {2, {{{5, 2}, {3, 4}, {1, 6}, {0, 7}}, {{7, 2}, {9, 4}, {10, 5}}}},
    {3, {{{7, 2}, {5, 4}, {3, 6}, {1, 8}, {0, 9}}, {{9, 2}, {10, 3}}}},
};

void logstate(vector<string> state, int energy) {
    int depth = state[0].length();
    cout << state[4] << "\t" << energy << endl;
    for (int i: range(0, depth))
        cout << "##" << state[0][i] << "#" << state[1][i] << "#" << state[2][i] << "#" << state[3][i] << "##" << endl;
    cout << "===========" << endl;
}

ulong f23a(const vector<string> &input, bool insertRows = true) {
    vector<string> state(5);
    vector<string> targetState{{"", "", "", "", "..........."}};

    // Additional rows for part 2
    vector<string> insert{"DD",  "CB", "BA", "AC"};

    for (int i: range(0, 4)) {
        state[i] = input[2].substr(1 + (i + 1) * 2, 1) + (insertRows ? insert[i] : "") + input[3].substr(1 + (i + 1) * 2, 1);
        targetState[i].resize(state[i].length(), 'A' + i);
    }
    state[4] = input[1].substr(1, input[1].length() - 2);

    map<vector<string>, int> reached;
    queue<pair<vector<string>, int>> q;
    q.push(pair{state, 0});
    reached[state] = 0;
    int iter = 0;
    while(q.size()) {
        ++iter;
        auto cur = q.front();
        auto [state, energy] = cur;
        q.pop();
        // logstate(state, energy);

        // generate all possible moves from the cur position and add into the queue
        // moves can not repeat so there is no need to check duplicate states
        
        // get out from caves
        for (int from: range(0, 4)) {
            int idx = state[from].find_first_of("ABCD");
            if (idx != string::npos) {
                char s0 = state[from][idx];
                int steps = idx;
                for (auto direction: moves[from]) {
                    for (auto cellSteps: direction) {
                        auto [i, s] = cellSteps;
                        if (state[4][i] != '.')
                            break;
                        vector<string> newState(state);
                        newState[from][idx] = '.';
                        newState[4][i] = s0;
                        int e = energy + (steps + s) * scores[s0];
                        // logstate(newState, e);
                        if (!reached.count(newState) || reached[newState] > e) {
                            q.push(pair{newState, e});
                            reached[newState] = e;
                        }
                    }
                }
            }
        }

        // jump into caves
        for (int from: range(0, 11)) {
            if (state[4][from] != '.') {
                char s0 = state[4][from];
                int cave = s0 - 'A';
                // cout << "from=" << from << " cave=" << cave << endl;
                int idx = -1;
                for (int i = 0; i < state[cave].length(); i++) {
                    if (state[cave][i] != '.' && state[cave][i] != s0) {
                        idx = -1;
                        break;
                    }
                    if (state[cave][i] == '.') {
                        idx = i;
                    }
                }
                if (idx >= 0) {
                    int to = 2 + 2 * (s0 - 'A');
                    int steps = 0;
                    for (int i: range(to, from)) {
                        if (state[4][i] != '.') {
                            steps = -1;
                            break;
                        } else {
                            ++steps;
                        }
                    }
                    if (steps >= 0) {
                        vector<string> newState(state);
                        newState[4][from] = '.';
                        newState[cave][idx] = s0;
                        int e = energy + (steps + idx + 1) * scores[s0];
                        // logstate(newState, e);
                        if (!reached.count(newState) || reached[newState] > e) {
                            q.push(pair{newState, e});
                            reached[newState] = e;
                        }
                    }
                }
            }
        }
        if (iter % 50000 == 0)
            cout << "Iteration " << iter << " states reached: " << reached.size() << endl;
    }
    cout << "Number of iterations=" << iter << endl;
    if (reached.count(targetState)) {
        cout << "Solution found, energy=" << reached[targetState] << endl;
        return reached[targetState];
    } else {
        cout << "No solution" << endl;
        return 0;
    }
}

int main() {
    ulong a = f23a(getContent("./input/input-23.txt"), false);
    cout << "23\t" << a << endl;
    assert(a == 14346);
    ulong b = f23a(getContent("./input/input-23.txt"), true);
    cout << "23a\t" << b << endl;
    assert(b == 48984);
    return 0;
}
