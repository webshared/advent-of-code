#include "common.hpp"

int f9(vector<string> input) {
    const int rows = input.size();
    const int cols = input[0].length();
    int res = 0;
    for (int i: range(0, rows)) {
        for (int j: range(0, cols)) {
            vector<int> vals;
            if (i > 0) vals.push_back(input[i - 1][j]);
            if (i < rows - 1) vals.push_back(input[i + 1][j]);
            if (j > 0) vals.push_back(input[i][j - 1]);
            if (j < cols - 1) vals.push_back(input[i][j + 1]);
            if (input[i][j] < *min_element(vals.begin(), vals.end())) {
                res += input[i][j] - '0' + 1;
            }
        }
    }
    return res;
}

int f9a(vector<string> input) {
    const int rows = input.size();
    const int cols = input[0].length();
    vector<int> basins; 
    for (int i: range(0, rows)) {
        for (int j: range(0, cols)) {
            vector<int> vals;
            if (i > 0) vals.push_back(input[i - 1][j]);
            if (i < rows - 1) vals.push_back(input[i + 1][j]);
            if (j > 0) vals.push_back(input[i][j - 1]);
            if (j < cols - 1) vals.push_back(input[i][j + 1]);
            if (input[i][j] < *min_element(vals.begin(), vals.end())) {
                queue<pair<int, int>> q;
                q.push(pair{i, j});
                int basinSize = 0;
                while(q.size()) {
                    pair<int, int> pos = q.front();
                    q.pop();
                    int x = pos.first;
                    int y = pos.second;
                    char v = input[x][y];
                    if (v != '.') {
                        ++basinSize;
                        if (x > 0 && input[x - 1][y] < '9') q.push(pair{x - 1, y});
                        if (x < rows - 1 && input[x + 1][y] < '9') q.push(pair{x + 1, y});
                        if (y > 0 && input[x][y - 1] < '9') q.push(pair{x, y - 1});
                        if (y < cols - 1 && input[x][y + 1] < '9') q.push(pair{x, y + 1});
                        input[x][y] = '.';
                    }
                }
                basins.push_back(basinSize);
            }
        }
    }
    sort(basins.begin(), basins.end());
    int top3 = accumulate(basins.end() - 3, basins.end(), 1, std::multiplies<int>());
    return top3;
}

int main() {
    cout << "9\t" << f9(getContent("./input/input-09.txt")) << endl;
    cout << "9a\t" << f9a(getContent("./input/input-09.txt")) << endl;
    return 0;
}
