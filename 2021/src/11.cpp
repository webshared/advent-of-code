#include "common.hpp"

int f11(vector<string> input, int steps = 100, bool stopOnAll = false) {
    int rows = input.size();
    int cols = input[0].length();
    int spikes = 0;
    for (int step: range(0, steps)) {
        // cout << "step " << step + 1 << endl;
        queue<pair<int, int>> q;
        for (int i: range(0, rows)) {
            for (int j: range(0, cols)) {
                q.push(pair{i, j});
            }
        }
        // for (string line: input) cout << line << endl; cout << endl;
        while (q.size()) {
            auto [x, y] = q.front();
            q.pop();
            char ch = input[x][y];
            char newCh = min(ch + 1, '9' + 1);
            input[x][y] = newCh;
            if (ch != newCh && newCh > '9') {
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if ((i != 0 || j != 0) && x + i >= 0 && y + j >= 0 && x + i < rows && y + j < cols) {
                          q.push(pair{x + i, y + j});
                        }
                    }
                }
            }
        }
        int stepSpikes = 0;
        for (int i: range(0, rows)) {
            for (int j: range(0, cols)) {
                if (input[i][j] > '9') {
                    input[i][j] = '0';
                    ++stepSpikes;
                }
            }
        }
        if (stepSpikes == rows * cols) return step + 1;
        spikes += stepSpikes;
        // for (string line: input) cout << line << endl; cout << endl;
    }
    return spikes; 
}

int f11a(vector<string> input) {
    return -1;    
};

int main() {
    cout << "11\t" << f11(getContent("./input/input-11.txt")) << endl;
    cout << "11a\t" << f11(getContent("./input/input-11.txt"), 1000, true) << endl;
    return 0;
}
