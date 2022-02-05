#include <chrono>
#include "common.hpp"

class Timer {
    std::chrono::_V2::steady_clock::time_point start;
    // std::chrono::time_point<std::chrono::system_clock> start;
    public:
    Timer() {
        this->start = std::chrono::steady_clock::now();
    }
    void log() {
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << std::endl;
    }
};

// generate 24 orientation matrices
vector<vector<vector<int>>> rotationMatrices() {
    vector<vector<vector<int>>> matrices;
    int c = 0;
    for (int x = 0; x < 6; ++x)
        for (int y = 0; y < 6; ++y)
            for (int z = 0; z < 6; ++z)
                if (x / 2 != y / 2 && y / 2 != z / 2 && x / 2 != z / 2) {
                    vector<vector<int>> m(3);
                    m[0].assign(3, 0);
                    m[1].assign(3, 0);
                    m[2].assign(3, 0);
                    m[0][x / 2] = x % 2 ? -1 : 1;
                    m[1][y / 2] = y % 2 ? -1 : 1;
                    m[2][z / 2] = z % 2 ? -1 : 1;
                    int det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                        - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
                        + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
                    if (det > 0) matrices.push_back(m);
                }
    return matrices;
}

vector<int> mmult(const vector<vector<int>> &rot, const vector<int> &coord) {
    vector<int> res(3);
    for (int i = 0; i < 3; i++)
        res[i] = rot[i][0] * coord[0] + rot[i][1] * coord[1] + rot[i][2] * coord[2];
    return res;
}

vector<vector<int>> mmult2(const vector<vector<int>> &rot, const vector<vector<int>> &coords) {
    vector<vector<int>> res(coords.size());
    for (int idx = 0; idx < coords.size(); ++idx) {
        res[idx] = mmult(rot, coords[idx]);
    }
    return res;
}

vector<int> mdiff(const vector<int> &a, const vector<int> &b) {
    vector<int> diff(3);
    for (int i: range(0, 3))
        diff[i] = a[i] - b[i];
    return diff;
}

bool meq(const vector<int> &a, const vector<int> &b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

void vlog(const vector<int> &v) {
    cout << "size=" << v.size() << " ";
    for (int val: v) cout << val << ",";
    cout << endl;
}

bool cmpCoords(const vector<int> &a, const vector<int> &b) {
    if (a[0] == b[0] && a[1] == b[1]) return a[2] < b[2];
    if (a[0] == b[0]) return a[1] < b[1];
    return a[0] < b[0];
}
 
int f19(const vector<string> &input) {
    const int minMatches = 12;
    vector<vector<vector<int>>> scanners;
    vector<vector<vector<int>>> matrices = rotationMatrices();

    // parse input
    int sIndex = -1;
    int scannedBeacons = 0;
    for (auto line: input) {
        auto vals = splitToInts(line, ',');
        if (vals.size() == 3) {
            scanners[sIndex].push_back(vals);
            ++scannedBeacons;
        } else {
            sIndex++;
            scanners.resize(sIndex + 1);
        }
    }

    int dups = 0;
    map<int, int> rotationIndices; // scannerId -> index in matrices if the scanner orientation is determined
    map<int, pair<int, vector<int>>> rotAndPos; // pair<rotationIndex, 3 coords of scanner position>
    rotAndPos.insert(pair{0, pair<int, vector<int>>{0, vector<int>{0, 0, 0}}});

    for (int i = 0; i < scanners.size(); ++i) {
        vector<vector<int>> iCoords(scanners[i].size());
        if (rotationIndices.count(i)) {
            iCoords = mmult2(matrices[rotationIndices[i]], scanners[i]);
        } else {
            iCoords.assign(scanners[i].begin(), scanners[i].end());
        }
        sort(iCoords.begin(), iCoords.end(), cmpCoords);
        for (int j = i + 1; j < scanners.size(); ++j) {
            // try to match scanners i and j
            cout << "Compare " << i << " vs. " << j << endl;
            vector<int> rotationMxIdxs;
            if (rotationIndices.count(j)) {
                cout << "rotation of " << j << " = " << rotationIndices[j] << endl;
                rotationMxIdxs.push_back(rotationIndices[j]);
            } else {
                cout << "rotation of " << j << " not determined\n";
                rotationMxIdxs = range(0, matrices.size());
            }
            for (int rotationMxIdx: rotationMxIdxs) {
                //cout << "M" << rotationMxIdx << endl;
                //Timer t;
                queue<vector<pair<int, int>>> q;
                for (int ii = 0; ii < iCoords.size(); ++ii)
                    for (int jj = 0; jj < scanners[j].size(); ++jj)
                        q.push(vector{pair{ii, jj}});
                bool found = false;
                vector<pair<int, int>> sequence;
                int hits = 0, iters = 0;
                while (q.size()) {
                    ++hits;
                    auto seq = q.front();
                    auto [beaconA, beaconB] = seq[seq.size() - 1];
                    set<int> usedBs;
                    for (auto &item: seq) usedBs.insert(item.second);
                    q.pop();
                    bool added = false;
                    int minToAdd = 0; // max(0, minMatches - (int)seq.size() - 1);
                    for (int ii = beaconA + 1; ii < iCoords.size() - minToAdd; ++ii) {
                        vector<int> dA = mdiff(iCoords[ii], iCoords[beaconA]);
                        for (int jj = beaconB + 1; jj < scanners[j].size(); ++jj) {
                            ++iters;
                            if (usedBs.count(jj) == 0) {
                                vector<int> dB = mdiff(jCoords[jj], jCoords[beaconB]);
                                if (meq(dA, dB)) {
                                    vector<pair<int, int>> newSeq;
                                    newSeq.assign(seq.begin(), seq.end());
                                    newSeq.push_back(pair{ii, jj});
                                    q.push(newSeq);
                                    added = true;
                                    if (newSeq.size() > 107) {
                                          cout << "size=" << newSeq.size() << endl;
                                          for (auto i: newSeq) {
                                              cout << "[" << i.first << " : " << i.second<< "] "; 
                                          }
                                          cout << "queue=" << q.size() << endl;
                                    }
                                    break;
                                }
                            }
                        }
                        if (added) break;
                    }
                    if (!added && seq.size() >= 12) {
                        // scanners i and j cover the same area
                        cout << " -- intersection found, orientation=" << rotationMxIdx << " points=" << seq.size() << endl;
                        dups += seq.size();
                        found = true;
                        for (auto i: seq) {
                            cout << "[" << i.first << " : " << i.second<< "] "; 
                        }
                        cout << endl;
                        sequence = seq;
                        rotationIndices.insert(pair{j, rotationMxIdx});
                        
                        vector<int> jScannerPos = mdiff(iCoords[seq[0].first], jCoords[seq[0].second]);
                        rotAndPos.insert(pair{j, pair{rotationMxIdx, jScannerPos}});
                        cout << "rotationMxIdx=" << rotationMxIdx << endl;
                        vlog(jScannerPos);
                        break;
                    }
                }
                //cout << "hits=" << hits << " iters=" << iters << " ratio=" << (iters / (hits + 1)) << endl;
                //t.log();
                if (found) break;
            }
        }
    }
    for (int i = 0; i < scanners.size(); ++i) {
        cout << "scanner " << i << " rot=" << (rotationIndices.count(i) ? to_string(rotationIndices[i]) : "not defined") << endl;
    }
    cout << "scannedBeacons=" << scannedBeacons << " dups=" << dups << endl;
    return scannedBeacons - dups;
}

int main() {
    cout << "19\t" << f19(getContent("./src/input-19.txt")) << endl;
    return 0;
}
