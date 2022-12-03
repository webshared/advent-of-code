#include "common.hpp"

string hex2bin(char ch) {
    int v = ch >= '0' && ch <= '9' ? ch - '0' : 10 + ch - 'A';
    string res;
    res += (v & 8) ? "1" : "0";
    res += (v & 4) ? "1" : "0";
    res += (v & 2) ? "1" : "0";
    res += (v & 1) ? "1" : "0";
    return res;
}

typedef unsigned long ulong;

ulong bin2dec(string str) {
    ulong v = 0;
    for (int i = 0; i < str.length(); i++) {
        v += (str[i] - '0');
        if (i < str.length() - 1)
          v <<= 1;
    }
    return v;
}

pair<int, ulong> f16(const vector<string> &input) {
    string str = input[0];
    string bits;
    for (char ch: str) {
        bits += hex2bin(ch);
    }
    cout << bits << endl;
    int verSum = 0;
    function<pair<int, ulong>(int, int)> readPck = [&bits, &verSum, &readPck] (int i, int level) -> pair<int, ulong> {
        string prefix;
        for(int tmp: range(0, level)) prefix += "|";
        int ver = bin2dec(bits.substr(i, 3));
        int typ = bin2dec(bits.substr(i + 3, 3));
        verSum += ver;
        ulong value = 0;
        // cout << prefix << "package start i=" << i << " total=" << bits.length() << " ver " << ver << " typ " << typ << endl;
        i += 6;
        if (typ == 4) {
            string valBits;
            for (;;) {
                int pckFlag = bin2dec(bits.substr(i, 1));
                valBits += bits.substr(i + 1, 4);
                i += 5;
                if (!pckFlag) // last package
                    break;
            }
            value = bin2dec(valBits);
            // cout << prefix << "literal value " << valBits << " =" << value << " len=" << valBits.length() << endl;
            cout << prefix << "=" << value << endl;
        } else {
            int lenTyp = bin2dec(bits.substr(i, 1));
            i++;
            // cout << prefix << "operator package lenTyp=" << lenTyp << endl;
            vector<ulong> values;
            if (lenTyp == 0) {
                int bitLen = bin2dec(bits.substr(i, 15));
                // cout << prefix << "subpcks length=" << bitLen << endl;
                i += 15;
                int lastBit = i + bitLen;
                while (i < lastBit) {
                    auto [pckI, pckVal] = readPck(i, level + 1);
                    i = pckI;
                    values.push_back(pckVal);
                }
            } else {
                int pckCount = bin2dec(bits.substr(i, 11));
                // cout << prefix << "subpcks count=" << pckCount << endl;
                i += 11;
                for (int pckIndex = 0; pckIndex < pckCount; ++pckIndex) {
                    // cout << prefix << "subpack=" << pckIndex << " i=" << i << endl;
                    auto [pckI, pckVal] = readPck(i, level + 1);
                    i = pckI;
                    values.push_back(pckVal);
                }
            }
            if (typ == 0) value = accumulate(values.begin(), values.end(), 0ul, std::plus<ulong>());
            if (typ == 1) value = accumulate(values.begin(), values.end(), 1ul, std::multiplies<ulong>());
            if (typ == 2) value = *min_element(values.begin(), values.end());
            if (typ == 3) value = *max_element(values.begin(), values.end());
            if (typ == 5) value = values[0] > values[1] ? 1 : 0;
            if (typ == 6) value = values[0] < values[1] ? 1 : 0;
            if (typ == 7) value = values[0] == values[1] ? 1 : 0;
            cout << prefix << "["; for(auto v: values) cout << v << ", "; cout << "] typ=" << typ << " => " << value << endl;
        }
        // cout << prefix << "package ended i=" << i << " value=" << value << endl;
        return pair{i, value};
    };

    auto [i, result] = readPck(0, 1);
    cout << "result=" << result << endl;
    return pair{verSum, result};
}

int main() {
    pair<int, ulong> res = f16(getContent("./input/input-16.txt"));
    cout << "16\t" << res.first << endl;
    cout << "16a\t" << res.second << endl;
    return 0;
}
