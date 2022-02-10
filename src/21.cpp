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
    return -1;
}

ulong f21a(const vector<string> &input, int stepsCnt) {
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
    return -1;
}

int main() {
    ulong a = f21(getContent("./input/input-21.txt"), 2);
    cout << "21\t" << a << endl;
    ulong b = f21a(getContent("./input/input-21.txt"), 2);
    cout << "21a\t" << b << endl;
    return 0;
}
