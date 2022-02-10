#include "common.hpp"

typedef vector<int> vec3d; // 3x1
typedef vector<vector<int>> rotMx; // 3x3
typedef vector<vector<int>> vecs3d; // 3xN

const int minMatches = 12;

// debug output helper
ostream& operator <<(ostream &out, vec3d &vec) {
    return out << "[" << vec[0] << "," << vec[1] << "," << vec[2] << "]";
}

// debug output helper
ostream& operator <<(ostream &out, vecs3d &m) {
    for (int i = 0; i < m.size(); i++)
        out << m[i];
    return out;
}

// generate 24 orientation matrices
// which are permutations of 3x3 matrices of -1, 0, and 1s where determinant equals 1
vector<rotMx> rotationMatrices() {
    vector<rotMx> matrices;
    int c = 0;
    for (int x = 0; x < 6; ++x)
        for (int y = 0; y < 6; ++y)
            for (int z = 0; z < 6; ++z) {
                vector<vector<int>> m(3);
                m[0].assign(3, 0);
                m[1].assign(3, 0);
                m[2].assign(3, 0);
                m[0][x / 2] = x % 2 ? -1 : 1;
                m[1][y / 2] = y % 2 ? -1 : 1;
                m[2][z / 2] = z % 2 ? -1 : 1;
                // determinant equals 1 for rotations
                int det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                    - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
                    + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
                if (det == 1) matrices.push_back(m);
            }
    return matrices;
}

vec3d mmult(const rotMx &rot, const vec3d &coord) {
    vec3d res(3);
    for (int i = 0; i < 3; i++)
        res[i] = rot[i][0] * coord[0] + rot[i][1] * coord[1] + rot[i][2] * coord[2];
    return res;
}

vecs3d mmult2(const rotMx &rot, const vecs3d &coords) {
    vecs3d res(coords.size());
    for (int idx = 0; idx < coords.size(); ++idx) {
        res[idx] = mmult(rot, coords[idx]);
    }
    return res;
}

rotMx mmult3d(const rotMx &mA, const rotMx &mB) {
    rotMx res(3);
    for (int i = 0; i < 3; i++) {
        res[i] = vec3d(3, 0);
        for (int j = 0; j < 3; j++) 
            for (int k = 0; k < 3; k++)
               res[i][j] += mA[i][k] * mB[k][j];
    }
    return res;
}

// Inverse rotation matrix is just a transpose for 90 degree rotations
rotMx minv(const rotMx &rot) {
    rotMx res(3);
    for (int i = 0; i < 3; i++)
        res[i] = vector(3, 0);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            res[j][i] = rot[i][j];
    return res;
}

vec3d madd(const vec3d &a, const vec3d &b) {
    vec3d diff(3);
    for (int i: range(0, 3))
        diff[i] = a[i] + b[i];
    return diff;
}

vec3d mdiff(const vec3d &a, const vec3d &b) {
    vec3d diff(3);
    for (int i: range(0, 3))
        diff[i] = a[i] - b[i];
    return diff;
}

bool meq(const vec3d &a, const vec3d &b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

bool cmpCoords(const vec3d &a, const vec3d &b) {
    if (a[0] == b[0] && a[1] == b[1]) return a[2] < b[2];
    if (a[0] == b[0]) return a[1] < b[1];
    return a[0] < b[0];
}

int numberOfDups(vecs3d &vecs) {
    sort(vecs.begin(), vecs.end(), cmpCoords);
    int dups = 0;
    for (int idx = 1; idx < vecs.size(); ++idx) {
        if (meq(vecs[idx - 1], vecs[idx])) {
            ++dups;
        }
    }
    return dups;
}

// Returns scanner B position in A's coordinate system if 12+ intersections found
optional<vec3d> findIntersection(
    const vecs3d &setA,
    const vecs3d &setB,
    const optional<reference_wrapper<rotMx>> rotAMx = std::nullopt,
    const optional<reference_wrapper<rotMx>> rotBMx = std::nullopt
) {
    vecs3d aCoords;
    aCoords = rotAMx ? mmult2(*rotAMx, setA) : setA;
    sort(aCoords.begin(), aCoords.end(), cmpCoords);
    vecs3d bCoords;
    bCoords = rotBMx ? mmult2(*rotBMx, setB) : setB;
    sort(bCoords.begin(), bCoords.end(), cmpCoords);
    for (int i = 0; i < aCoords.size(); ++i) {
        const vec3d &shiftA = aCoords[i];
        for (int j = 0; j < bCoords.size(); ++j) {
            const vec3d &shiftB = bCoords[j];
            vecs3d allShifted;
            for (vec3d vec: aCoords)
                allShifted.push_back(mdiff(vec, shiftA));
            for (vec3d vec: bCoords)
                allShifted.push_back(mdiff(vec, shiftB));
            int matches = numberOfDups(allShifted);
            if (matches >= minMatches) {
                return make_optional(mdiff(shiftA, shiftB));
            }
        }
    }
    return std::nullopt;
} 

pair<int, int> f19(const vector<string> &input) {
    vector<vecs3d> scanners;
    vector<rotMx> matrices = rotationMatrices();

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

    // here we have a scanners graf where edges are transitions of coordinate systems: pair<rotMx, vec3d>
    // build the adjacency matrix
    vector<vector<optional<pair<rotMx, vec3d>>>> adj(scanners.size());
    for (int i = 0; i < scanners.size(); ++i)
        adj[i] = vector<optional<pair<rotMx, vec3d>>>(scanners.size(), std::nullopt);
    for (int i = 0; i < scanners.size(); ++i) {
        adj[i][i] = make_optional(pair{matrices[0], vec3d{0, 0, 0}});
        for (int j = i + 1; j < scanners.size(); ++j) {
            cout << "CMP " << i << " vs " << j << endl;
            for (int rot = 0; rot < matrices.size(); ++rot) {
                auto coordOfB = findIntersection(
                    scanners[i],
                    scanners[j],
                    std::nullopt,
                    matrices[rot]
                );
                if (coordOfB) {
                    cout << "match rotIdx=" << rot << " coord=" << *coordOfB << " rotation=" << matrices[rot] << endl;
                    // scanner j positoin from i's viewpoint
                    adj[i][j] = make_optional(pair{matrices[rot], *coordOfB});
                    // scanner i positoin from j
                    rotMx rotInv = minv(matrices[rot]);
                    adj[j][i] = make_optional(pair{rotInv, mmult(rotInv, mdiff(vec3d{0, 0, 0}, *coordOfB))});
                }
            }
        }
    }

    // print an overview of adjacency matrix
    for (int i = 0; i < scanners.size(); ++i) {
        for (int j = 0; j < scanners.size(); ++j)
            cout << (adj[i][j] ? "#" : "-");
        cout << endl;
    }

    // now build the reachability matrix to find how all scanners are translated into 0's coordinates
    vector<vector<optional<pair<rotMx, vec3d>>>> dst(adj.begin(), adj.end());
    for (int k = 0; k < scanners.size(); ++k) {
        for (int i = 0; i < scanners.size(); ++i) {
            for (int j = 0; j < scanners.size(); ++j) {
                if (!dst[i][j] && dst[i][k] && dst[k][j]) {
                    // there is no edge i->j, but there is i->k, k->j
                    // recall some linear algebra to calculate transition matrices
                    rotMx mA = dst[i][k]->first;
                    rotMx mB = dst[k][j]->first;
                    vec3d a = dst[i][k]->second;
                    vec3d b = dst[k][j]->second;
                    dst[i][j] = make_optional(pair{
                        mmult3d(mA, mB),
                        madd(a, mmult(mA, b))
                    });
                }
            }
        }
    }

    auto convertCoords = [&dst, &scanners] (int from, int to, const vecs3d &vecs, vecs3d &converted) -> vecs3d {
        rotMx &rot = dst[from][to]->first;
        vec3d &origin = dst[from][to]->second;
        for (auto v: vecs) {
            vec3d v0 = madd(origin, mmult(rot, v));
            converted.push_back(v0);
        }
        return converted;
    };

    // an easy part:
    // convert all points into scanner 0 cooridate system and find number of dups
    vecs3d allPoints;
    for (int i = 0; i < scanners.size(); ++i)
        convertCoords(0, i, scanners[i], allPoints);
    int dups = numberOfDups(allPoints);
    int uniques = allPoints.size() - dups;
    cout << "\nALL POINTS\n" << allPoints << endl;

    // cancluate max. distance between scanners
    int maxDist = 0;
    for (int i = 0; i < scanners.size(); ++i) {
        for (int j = 0; j < scanners.size(); ++j) {
            vec3d diff = mdiff(dst[0][i]->second, dst[0][j]->second);
            int d = abs(diff[0]) + abs(diff[1]) + abs(diff[2]);
            if (maxDist < d)
                maxDist = d;
        }
    }

    return pair<int, int>{uniques, maxDist};
}

int main() {
    auto res = f19(getContent("./input/input-19.txt"));
    cout << "19\t" << res.first << endl;
    cout << "19a\t" << res.second << endl;
    return 0;
}
