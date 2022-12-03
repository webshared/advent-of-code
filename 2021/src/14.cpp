#include "common.hpp"

map<string, string> loadF14(const vector<string> &input) {
    boost::regex expression("([A-Z]+) -> ([A-Z]+)");
    map<string, string> m;
    for (int i = 1; i < input.size(); i++) {
        boost::cmatch what;
        if(boost::regex_match(input[i].c_str(), what, expression)) {
            string from(what[1].first, what[1].second);
            string to(what[2].first, what[2].second);
            m.insert(pair{from, to});
        }
    }
    return m;
};

unsigned long f14(vector<string> input) {
    string str = input[0];
    map<string, string> m = loadF14(input);
    for (int step: range(0, 10)) {
        string res;
        for (int i = 0; i < str.length(); i++) {
            string match = str.substr(i, 2);
            if (m.count(match)) {
                res += str[i] + m[match];
            } else {
                res += str[i];
            }
        }
        cout << "step " << step + 1 << " len:" << res.length() << endl;
        str = res;
    }
    map<char, unsigned long> counts;
    for (char ch: str) {
        if (counts.count(ch))
            counts[ch]++;
        else
            counts.insert(pair{ch, 1});
    }
    vector<unsigned long> freqs;
    std::transform(counts.begin(), counts.end(), std::back_inserter(freqs), [](pair<char, unsigned long> p) { return p.second; });
    unsigned long res = *max_element(freqs.begin(), freqs.end()) - *min_element(freqs.begin(), freqs.end());
    return res;
}

unsigned long f14a(vector<string> input) {
    string str = input[0];
    map<string, string> m = loadF14(input);

    // pairs are <2-chars sequence>, <count of occurencies>
    map<string, unsigned long> pairs;
    map<char, unsigned long> counts;

    auto upsertPair = [] (map<string, unsigned long> &pairs, string two, unsigned long cnt = 1) {
        if (pairs.count(two)) pairs[two] += cnt;
        else pairs.insert(pair{two, cnt});
    };

    for (char ch: str) {
        if (counts.count(ch)) counts[ch]++;
        else counts.insert(pair{ch, 1});
    }
    for (int i = 0; i < str.length() - 1; i++) {
        string two = str.substr(i, 2);
        upsertPair(pairs, two);
    }

    for (int step: range(0, 40)) {
        map<string, unsigned long> newPairs;
        for (auto [two, cnt]: pairs) {
            if (m.count(two)) {
                string p1 = two[0] + m[two];
                upsertPair(newPairs, two[0] + m[two], cnt);
                upsertPair(newPairs, m[two] + two[1], cnt);
                counts[m[two][0]] += cnt;
            }
        }
        unsigned long total = 0;
        for (auto [ch, cnt]: counts) {
            total += cnt;
        }
        cout << "step " << step + 1 << " len:" << total << endl;
        pairs = newPairs;
    }
    vector<unsigned long> freqs;
    transform(counts.begin(), counts.end(), std::back_inserter(freqs), [](pair<char, unsigned long> p) { return p.second; });
    unsigned long res = *max_element(freqs.begin(), freqs.end()) - *min_element(freqs.begin(), freqs.end());
    return res;
}

int main() {
    cout << "14\t" << f14(getContent("./input/input-14.txt")) << endl;
    cout << "14a\t" << f14a(getContent("./input/input-14.txt")) << endl;
    return 0;
}
