#include "common.hpp"

vector<vector<int>> load(const vector<string> &input) {
    using namespace boost;
    vector<vector<int>> vals;
    for (auto &line: input) {
        regex expression("(on|off) x=([-\\d]+)..([-\\d]+),y=([-\\d]+)..([-\\d]+),z=([-\\d]+)..([-\\d]+)");
        boost::match_results<std::string::const_iterator> what;
        if (regex_search(line.begin(), line.cend(), what, expression)) {
            int x0 = stoi(string(what[2].first, what[2].second));
            int x1 = stoi(string(what[3].first, what[3].second));
            int y0 = stoi(string(what[4].first, what[4].second));
            int y1 = stoi(string(what[5].first, what[5].second));
            int z0 = stoi(string(what[6].first, what[6].second));
            int z1 = stoi(string(what[7].first, what[7].second));
            int on = line.substr(0, 2) == "on" ? 1 : 0;
            vals.push_back(vector<int>{x0, x1, y0, y1, z0, z1, on});
            // cout << (on ? "ON" : "OFF") << " x=" << x0 << "," << x1 << " y=" << y0 << "," << y1 << " z=" << z0 << "," << z1 << endl;
        }
    }
    return vals;
}

ulong f22(const vector<string> &input) {
    const int n = 101;
    vector<int> bits(n * n * n, 0);
    auto vals = load(input);
    for (auto &v: vals) {
        if (v[0] >= -50 && v[1] <= 50) {
            for (int i: range(v[0], v[1], true))
                for (int j: range(v[2], v[3], true))
                    for (int k: range(v[4], v[5], true))
                        bits[n * n * (i + 50) + n * (j + 50) + (k + 50)] = v[6];
        }
    }
    return accumulate(bits.begin(), bits.end(), 0);
}

ulong f22a(const vector<string> &input) {
    auto vals = load(input);
    auto uniqSortedWithMap = [&vals] (int i0, int i1) {
        vector<int> coords;
        map<int, int> mapped;
        for (int i = 0; i < vals.size(); ++i) {
            coords.push_back(vals[i][i0]);
            coords.push_back(vals[i][i1]);
        }
        sort(coords.begin(), coords.end());
        vector<int>::iterator iter = unique(coords.begin(), coords.end());
        coords.resize(distance(coords.begin(), iter));
        for (int i = 0; i < coords.size(); ++i) {
            mapped.insert(pair{coords[i], i});
        }
        return pair{coords, mapped};
    };
    auto [xs, xmapped] = uniqSortedWithMap(0, 1);
    auto [ys, ymapped] = uniqSortedWithMap(2, 3);
    auto [zs, zmapped] = uniqSortedWithMap(4, 5);

    ulong xn = xs.size(), yn = ys.size(), zn = zs.size();

    // pack values in bits to save space (~500Mb of ram for problem input)
    // there must be a better solution for sparse bits vector, but it works :)
    ulong bitsPerItem = 8;
    ulong size = ((2 * xn - 1) * (2 * yn - 1) * (2 * zn - 1) + bitsPerItem - 1) / bitsPerItem;
    cout << xn << "*" << yn << "*" << zn << " = " << size << " bytes allocated" << endl;
    vector<unsigned char> bits(size, 0);

    auto getIdx = [&bits, xn, yn, zn, bitsPerItem] (int i, int j, int k) -> pair<ulong, int> {
        ulong rawIdx = (2 * yn - 1) * (2 * zn - 1) * i + (2 * zn - 1) * j + k;
        return pair{rawIdx / bitsPerItem, rawIdx % bitsPerItem};
    };

    auto getBitAt = [&bits, &getIdx, xn, yn, zn] (int i, int j, int k) -> int {
        auto [idx, bitIdx] = getIdx(i, j, k);
        return bits[idx] & (1 << bitIdx) ? 1 : 0;
    };

    auto setBitAt = [&bits, &getIdx, xn, yn, zn] (int i, int j, int k, int value) {
        auto [idx, bitIdx] = getIdx(i, j, k);
        if (value)
            bits[idx] = bits[idx] | (1 << bitIdx);
        else
            bits[idx] = bits[idx] & ~(1 << bitIdx);
    };

    auto mappedRange = [] (map<int, int> &mapped, int val1, int val2) {
        return range(
            2 * min(mapped[val1], mapped[val2]),
            2 * max(mapped[val1], mapped[val2]),
            true
        );
    };

    for (auto v: vals) {
        for (int i: mappedRange(xmapped, v[0], v[1]))
            for (int j: mappedRange(ymapped, v[2], v[3]))
                for (int k: mappedRange(zmapped, v[4], v[5])) {
                    //cout << "assign: " << i << " " << j << " " << k << " <- " << v[6] << endl;
                    setBitAt(i, j, k, v[6]);
                }
    }

    ulong cnt = 0;
    for (int i: range(0, 2 * xn - 1))
        for (int j: range(0, 2 * yn - 1))
            for (int k: range(0, 2 * zn - 1))
                if (getBitAt(i, j, k)) {
                    ulong di = (i % 2) ? (xs[(i + 1) / 2] - xs[i / 2] - 1) : 1;
                    ulong dj = (j % 2) ? (ys[(j + 1) / 2] - ys[j / 2] - 1) : 1;
                    ulong dk = (k % 2) ? (zs[(k + 1) / 2] - zs[k / 2] - 1) : 1;
                    ulong volume = di * dj * dk;
                    //cout << "vol: " << i << " " << j << " " << k << " = " << volume << endl;
                    cnt += volume;
                }
    return cnt;
}

int main() {
    ulong a = f22(getContent("./input/input-22.txt"));
    cout << "22\t" << a << endl;
    ulong b = f22a(getContent("./input/input-22.txt"));
    cout << "22a\t" << b << endl;
    return 0;
}
