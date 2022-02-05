#include "common.hpp"

int f8(vector<string> input) {
    int res = 0;
    for (string line: input) {
        auto parts = split(line, '|');
        if (parts.size()) {
            auto second = split(parts[1], ' ');
            for (auto w: second) {
                int s = w.length();
                if (s == 2 || s == 3 || s == 4 || s == 7)
                  ++res;
            }
        }
    }
    return res;
}

int f8a(vector<string> input) {
    vector<string> digits = {"abcefg", "cf", "acdeg", "acdfg", "bcdf", "abdfg", "abdefg", "acf", "abcdefg", "abcdfg"};
    vector<string> encoded;
    auto decode = [](const string &encoded, const string &key) {
        string decoded = "";
        for (int i = 0; i < encoded.length(); ++i) {
            decoded += key[(int)(encoded[i] - 'a')] - '0' + 'a';
        }
        sort(decoded.begin(), decoded.end());
        return decoded;
    };
    auto test = [&encoded, &digits, &decode](const string &key) -> bool {
        for (string enc: encoded) {
            string decoded = decode(enc, key);
            if (find(digits.begin(), digits.end(), decoded) == digits.end()) {
                return false; 
            }
        }
        return true;
    };
    function<bool(string&, int, int)> permutate = [&permutate, &test](string &str, int l, int r) -> bool {
        if (l == r) {
            return !test(str);
        } else {
            for (int i = l; i <= r; ++i) {
                swap(str[l], str[i]);
                if (!permutate(str, l + 1, r)) {
                    return false;
                }
                swap(str[l], str[i]);
            }
        }
        return true;
    };
    int res = 0;
    for (string line: input) {
        auto parts = split(line, '|');
        if (parts.size()) {
            encoded = split(parts[0], ' ');
            auto second = split(parts[1], ' ');
            string key = "0123456";
            permutate(key, 0, key.length() - 1);
            //cout << "solution: " << key << endl;
            int d = 0;
            for (string digit: second) {
                string decoded = decode(digit, key);
                auto it = find(digits.begin(), digits.end(), decoded);
                d = d * 10 + distance(digits.begin(), it);
            }
            //cout << "D " << d << endl;
            res += d;
        }
    }
    return res;
}

int main() {
    cout << "8\t" << f8(getContent("./input/input-08.txt")) << endl;
    cout << "8a\t" << f8a(getContent("./input/input-08.txt")) << endl;
    return 0;
}
