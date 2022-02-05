#include "common.hpp"

int f15(const vector<string> &input) {
    const int cols = input[0].length();
    const int rows = input.size();

    vector<vector<bool>> inqueue;
    inqueue.assign(rows, vector<bool>(cols, 0));

    queue<pair<int, int>> visited;
    visited.push(pair{0, 0});
    inqueue[0][0] = true;

    vector<vector<int>> ws(rows);
    ws.assign(rows, vector<int>(cols, 0));

    auto prnW = [&ws] () {
        for (int i = 0; i < ws.size(); i++) {
            for (int j = 0; j < ws[i].size(); j++)
                cout << ws[i][j] << "\t";
            cout << endl;
        }
        cout << endl;
    };
    // prnW();
    cout << "rows " << rows << " cols " << cols << endl;
    const vector<pair<int, int>> deltas = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    while (visited.size()) {
        auto [x, y] = visited.front();
        visited.pop();
        inqueue[y][x] = false;
        if (x != 0 || y != 0 || x != cols - 1 || y != rows - 1) {
            // any position except start and end
            for (auto [dx, dy]: deltas) {
                if (x + dx >= 0 && x + dx < cols && y + dy >= 0 && y + dy < rows) {
                    int w = ws[y][x];
                    int newW = w + stoi(input[y + dy].substr(x + dx, 1));
                    if (ws[y + dy][x + dx] <= 0 || newW < ws[y + dy][x + dx]) {
                        // we found a better way to the cell
                        ws[y + dy][x + dx] = newW;
                        if (!inqueue[y + dy][x + dx]) {
                            inqueue[y + dy][x + dx] = false;
                            visited.push(pair{x + dx, y + dy});
                        }
                    }
                }
            }
        }
        // prnW();
    }
    return ws[rows - 1][cols - 1];
}

int f15a(const vector<string> &input) {
    auto shifted = [](string line, int shift) {
        string lineTiled;
        for (char ch: line) {
            char t = (char)((ch - '1' + shift) % 9 + '1');
            lineTiled = lineTiled + t;
        }
        return lineTiled;
    };
    const int cols = input[0].length();
    const int rows = input.size();
    const int N = 5;
    vector<string> tiled(N * rows);
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            for (int lineIndex: range(0, rows)) {
                tiled[j * rows + lineIndex] += shifted(input[lineIndex], i + j);
            }
        }
    }
    return f15(tiled);
}

int main() {
    cout << "15\t" << f15(getContent("./input/input-15.txt")) << endl;
    cout << "15a\t" << f15a(getContent("./input/input-15.txt")) << endl;
    return 0;
}
