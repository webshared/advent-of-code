#include "common.hpp"

typedef unsigned long ulong;

ulong f21(const vector<string> &input, int stepsCnt) {
    int startA = input[0].back() - '0';
    int startB = input[1].back() - '0';
    int pointsA = 0, pointsB = 0;
    int a = startA, b = startB;
    for (int roll = 0; ; ++roll) {
        int dice = (roll * 3 + 1) + (roll * 3 + 2) + (roll * 3 + 3);
        if (roll % 2 == 0) {
            a = ((a - 1 + dice) % 10) + 1;
            pointsA += a;
        } else {
            b = ((b - 1 + dice) % 10) + 1;
            pointsB += b;
        }
        cout << "roll=" << (roll + 1) * 3 << " a=" << a << " b=" << b << " pointsA=" << pointsA << " pointsB=" << pointsB << endl;
        if (pointsA >= 1000) {
            return pointsB * (roll + 1) * 3;
        } else if (pointsB >= 1000) {
            return pointsA * (roll + 1) * 3;
        }
    }
    return 0;
}

typedef std::tuple<vector<int>, vector<int>, int> pos_ab; // positions, points, current player index
ostream& operator << (ostream &out, pos_ab pos) {
    auto [ab, pts, curIdx] = pos;
    out << ab[0] << ":" << ab[1] << "\t" << pts[0] << ":" << pts[1] << "\t" << curIdx << "\t";
    return out;
}

ulong f21a(const vector<string> &input, int stepsCnt) {
    int startA = input[0].back() - '0';
    int startB = input[1].back() - '0';
    pos_ab start{{startA, startB}, {0, 0}, 0}; // 10 x 10 x 21 x 21 = 44,100

    // each 3x roll outcomes are <total dice value> => <number of occurencies.
    vector<int> outcomes(10, 0);
    for (int roll1: range(1, 3, true))
        for (int roll2: range(1, 3, true))
            for (int roll3: range(1, 3, true))
                ++outcomes[roll1 + roll2 + roll3];

    const int winScore = 21;
    auto pos_cmp = [] (pos_ab &a, pos_ab &b) {
        return get<1>(a)[0] + get<1>(a)[1] > get<1>(b)[0] + get<1>(b)[1];
    };
    priority_queue<pos_ab, vector<pos_ab>, decltype(pos_cmp)> q(pos_cmp);
    q.push(start);
    map<pos_ab, ulong> posCnts;
    posCnts.insert(pair{start, 1ul});
    vector<ulong> totals(2, 0ul);
    while (!q.empty()) {
        auto p = q.top();
        q.pop();
        ulong cnt = posCnts[p];
        for (int dice = 0; dice < outcomes.size(); dice++) {
            if (outcomes[dice]) {
                auto [ab, pts, curIdx] = p;
                ulong totalOutcomes = cnt * outcomes[dice];
                ab[curIdx] = ((ab[curIdx] - 1 + dice) % 10) + 1;
                pts[curIdx] += ab[curIdx];
                if (pts[curIdx] >= winScore) {
                    totals[curIdx] += totalOutcomes;
                } else {
                    pos_ab newPos{ab, pts, curIdx ? 0 : 1};
                    if (posCnts.count(newPos)) {
                        posCnts[newPos] += totalOutcomes;
                    } else {
                        posCnts.insert(pair{newPos, totalOutcomes});
                        q.push(newPos);
                    }
                }
            }
        }
    }
    cout << "totals: " << totals[0] << "\t" << totals[1] << endl;
    return totals[0] > totals[1] ? totals[0] : totals[1];
}

int main() {
    ulong a = f21(getContent("./input/input-21.txt"), 2);
    cout << "21\t" << a << endl;
    ulong b = f21a(getContent("./input/input-21.txt"), 2);
    cout << "21a\t" << b << endl;
    return 0;
}
