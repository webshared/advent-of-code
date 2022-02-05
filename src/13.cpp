#include "common.hpp"

int f13(vector<string> input, bool first = true) {
    using namespace boost;
    vector<int> xs;
    vector<int> ys;
    vector<pair<char, int>> folds;
    for (string str: input) {
        auto v = splitToInts(str, ',');
        if (v.size() >= 2) {
            xs.push_back(v[0]);
            ys.push_back(v[1]);
        } else {
            regex expression("fold along ([xy])=([0-9]+)");
            cmatch what;
            if(regex_match(str.c_str(), what, expression)) {
                  string xy(what[1].first, what[1].second);
                  string valueStr(what[2].first, what[2].second);
                  folds.push_back(pair<char, int>{xy[0], stoi(valueStr)});
            }
        }
    }
    auto matrix = [&xs, &ys]() -> vector<string> {
        int X = *max_element(xs.begin(), xs.end()) + 1;
        int Y = *max_element(ys.begin(), ys.end()) + 1;
        vector<string> m(Y);
        for (auto &str: m)
            str.resize(X, '.');
        for (int i = 0; i < xs.size(); i++) {
            m[ys[i]][xs[i]] = '#';
        }
        return m;
    };
    auto prnM = [](const vector<string> &m) {
      for (int i = 0; i < m.size(); i++) cout << m[i] << endl; cout << endl;
    };
    auto calcM = [](const vector<string> &m) {
        int res = 0;
        for (const auto &row: m)
            for (int j = 0; j < row.length(); j++ )
                res += row[j] == '#' ? 1 : 0;
        return res;
    };
    for (auto [what, val]: folds) {
        if (what == 'x') {
            for (int i = 0; i < xs.size(); i++)
                if (xs[i] >= val)
                    xs[i] = val - (xs[i] - val);
        } else {
            for (int i = 0; i < ys.size(); i++)
                if (ys[i] >= val)
                    ys[i] = val - (ys[i] - val);
        }
        if (first) {
            auto m = matrix();
            return calcM(m);
        }
    }
    auto m = matrix();
    prnM(m);
    return -1;
}

int main() {
    cout << "13\t" << f13(getContent("./input/input-13.txt")) << endl;
    cout << "13a\n";
    f13(getContent("./input/input-13.txt"), false);
    return 0;
}
