#include "common.hpp"

int f2(vector<string> input) {
  using namespace boost;
  int x = 0, y = 0;
  regex expression("([a-z]+)\\s+([0-9]+)");
  for (int i = 0; i < input.size(); i++) {
      cmatch what;
      if(regex_match(input[i].c_str(), what, expression)) {
          string cmd(what[1].first, what[1].second);
          string valueStr(what[2].first, what[2].second);
          int value = atoi(valueStr.c_str());
          if (cmd == "forward") {
              x += value;
          }
          if (cmd == "down") {
              y += value;
          }
          if (cmd == "up") {
              y -= value;
          }
      }
  }
  return x * y;
}

int f2a(vector<string> input) {
  using namespace boost;
  int x = 0, y = 0, aim = 0;
  regex expression("([a-z]+)\\s+([0-9]+)");
  for (int i = 0; i < input.size(); i++) {
      cmatch what;
      if(regex_match(input[i].c_str(), what, expression)) {
          string cmd(what[1].first, what[1].second);
          string valueStr(what[2].first, what[2].second);
          int value = atoi(valueStr.c_str());
          if (cmd == "forward") {
              x += value;
              y += aim * value;
          }
          if (cmd == "down") {
              aim += value;
          }
          if (cmd == "up") {
              aim -= value;
          }
      }
  }
  return x * y;
}

int f3(vector<string> input) {
  vector<int> pBits;
  for (int i = 0; i < input.size(); i++) {
        for (int pos = 0; pos < input[i].length(); pos++) {
          if (pBits.size() <= pos) pBits.push_back(0);
          if (input[i].at(pos) == '1') pBits[pos] = pBits[pos] + 1;
        }
        // cout << input[i] << " " << input[i].length() << " " << pBits.size() << endl;
    }
    int gamma = 0, eps = 0;
    for (int pos = 0; pos < pBits.size(); pos++) {
        if (pBits[pos] > input.size() / 2) gamma |= 1;
        gamma <<= 1;
    }
    gamma >>= 1;
    eps = ~gamma;
    return gamma * (ones(pBits.size()) & eps);
}

int f3a(vector<string> input) {
  auto calc = [input] (bool isO2) -> int {
    set<int> idx;
    int maxBits = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i].length()) {
          maxBits = input[i].length();
          idx.insert(i);
        }
    }
    for (int bitIndex = 0; idx.size() > 1 && bitIndex < maxBits; ++bitIndex) {
      int onesCnt = 0;
      for (auto it = idx.begin(); it != idx.end(); ++it) {
          if (input[*it].at(bitIndex) == '1') onesCnt++;
      }
      bool onesPrevail = onesCnt * 2 >= idx.size(); 
      char criteria = isO2 ? (onesPrevail ? '1' : '0') : (onesPrevail ? '0' : '1');
      set<int> filteredIdx;
      for (auto it = idx.begin(); it != idx.end(); ++it) {
          if (input[*it].at(bitIndex) == criteria) 
          filteredIdx.insert(*it);
      }
      idx = filteredIdx;
    }
    return fromBits(input[*idx.begin()]);
  };
  return calc(true) * calc(false);
}

vector<int> _f4(vector<string> input) {
    vector<string> nums = split(input[0], ',');
    const int K = 5;
    int n = (input.size() - 1) / K;
    vector<vector<int>> boards; // shape of [N * K; K]
    vector<vector<bool>> marked;
    map<int, set<pair<int, int>>> refs;
    for (int i = 1; i < input.size(); i++) {
        vector<string> vals = split(input[i], ' ');
        vector<int> intvals;
        for (int j = 0; j < K; j++) {
            int value = stoi(vals[j]);
            intvals.push_back(value);
            if (refs.count(value)) {
                refs[value].insert(pair{i - 1, j});
            } else {
                set<pair<int, int>> s;
                s.insert(pair{i - 1, j});
                refs.insert(pair{value, s});
            }
        }
        if (!intvals.empty()) {
            boards.push_back(intvals);
            vector<bool> markedRow(K);
            fill(markedRow.begin(), markedRow.end(), false);
            marked.push_back(markedRow);
        }
    }
    set<int> won;
    vector<int> scores;
    for (auto &cur: nums) {
        int value = stoi(cur);
        if (refs.count(value)) {
            for (auto &pair: refs[value]) {
                int row = K * (pair.first / K);
                if (!won.count(row)) {
                    marked[pair.first][pair.second] = true;
                    bool rowMatch = true;
                    for (int i: range(0, K)) {
                        rowMatch &= marked[pair.first][i];
                    }
                    bool colMatch = true;
                    for (int i: range(row, row + K)) {
                        colMatch &= marked[i][pair.second];
                    }
                    if (rowMatch || colMatch) {
                        int res = 0;
                        for (int i: range(row, row + K)) {
                            for (int j: range(0, K)) {
                                if (!marked[i][j]) {
                                    res += boards[i][j];
                                }
                            }
                        }
                        won.insert(row);
                        scores.push_back(res * value);
                    }
                }
            }
        }
    }
    return scores;
}

int f4(vector<string> input) {
    return _f4(input).front();
}

int f4a(vector<string> input) {
    return _f4(input).back();
}

int f5(vector<string> input) {
    boost::regex expression("(\\d+),(\\d+) -> (\\d+),(\\d+)");
    const int N = 1000;
    vector<int> B(N * N);
    fill(B.begin(), B.end(), 0);
    for (int i = 0; i < input.size(); i++) {
        boost::cmatch what;
        if(boost::regex_match(input[i].c_str(), what, expression)) {
            int fromX = stoi(string(what[1].first, what[1].second));
            int fromY = stoi(string(what[2].first, what[2].second));
            int toX = stoi(string(what[3].first, what[3].second));
            int toY = stoi(string(what[4].first, what[4].second));
            if (fromX == toX) {
                for (int y: range(fromY, toY, true)) {
                    ++B[y * N + fromX];
                }
            }
            else if (fromY == toY) {
                for (int x: range(fromX, toX, true)) {
                    ++B[fromY * N + x];
                }
            }
        }
    }
    int res = 0;
    for (int val: B) {
        res += val > 1 ? 1 : 0;
    }
    return res;
}

int f5a(vector<string> input) {
    boost::regex expression("(\\d+),(\\d+) -> (\\d+),(\\d+)");
    const int N = 1000;
    vector<int> B(N * N);
    fill(B.begin(), B.end(), 0);
    for (int i = 0; i < input.size(); i++) {
        boost::cmatch what;
        if(boost::regex_match(input[i].c_str(), what, expression)) {
            int fromX = stoi(string(what[1].first, what[1].second));
            int fromY = stoi(string(what[2].first, what[2].second));
            int toX = stoi(string(what[3].first, what[3].second));
            int toY = stoi(string(what[4].first, what[4].second));
            if (fromX == toX) {
                for (int y: range(fromY, toY, true)) {
                    ++B[y * N + fromX];
                }
            }
            else if (fromY == toY) {
                for (int x: range(fromX, toX, true)) {
                    ++B[fromY * N + x];
                }
            } else {
                auto xs = range(fromX, toX, true);
                auto ys = range(fromY, toY, true);
                for (int i: range(0, xs.size())) {
                    ++B[ys[i] * N + xs[i]];
                }
            }
        }
    }
    int res = 0;
    for (int val: B) {
        res += val > 1 ? 1 : 0;
    }
    return res;
}

int f6(vector<string> input) {
    vector<int> nums = splitToInts(input[0], ',');
    for (int i: range(0, 80)) {
      for (int index = nums.size() - 1; index >= 0; --index) {
          int t = nums[index] - 1;
          if (t < 0) {
              nums[index] = 6;
              nums.push_back(8);
          } else {
              nums[index] = t;
          }
      }
    }
    return nums.size();
}

unsigned long f6a(vector<string> input) {
    vector<int> nums = splitToInts(input[0], ',');
    unsigned long vals[9] = {0, 0, 0,  0, 0, 0,  0, 0, 0};
    unsigned long sum = 0;
    int offset = 0;
    for (int num: nums) {
        ++vals[num];
        ++sum;
    }
    for (int day: range(0, 256)) {
    ++offset;
    unsigned long newCnt = vals[(offset + 8) % 9];
    sum += newCnt;
    vals[(offset + 6) % 9] += newCnt;
    }
    return sum;
}

int f7(vector<string> input) {
    vector<int> nums = splitToInts(input[0], ',');
    vector<int> all = range(
        *min_element(nums.begin(), nums.end()),
        *max_element(nums.begin(), nums.end()),
        true
    );
    vector<int> fuels;
    for (int i: all) {
        int fuel = 0;
        for (int x: nums) {
            fuel += abs(x - i);
        }
        fuels.push_back(fuel);
    }
    return *min_element(fuels.begin(), fuels.end());
}

int f7a(vector<string> input) {
    vector<int> nums = splitToInts(input[0], ',');
    vector<int> all = range(
        *min_element(nums.begin(), nums.end()),
        *max_element(nums.begin(), nums.end()),
        true
    );
    vector<int> fuels;
    for (int i: all) {
        int fuel = 0;
        for (int x: nums) {
            int dist = abs(x - i);
            fuel += dist * (1 + dist) / 2;
        }
        fuels.push_back(fuel);
    }
    return *min_element(fuels.begin(), fuels.end());
}

int main() {
    cout << "2\t" << f2(getContent("./input/input-02.txt")) << endl;
    cout << "2a\t" << f2a(getContent("./input/input-02.txt")) << endl;
    cout << "3\t" << f3(getContent("./input/input-03.txt")) << endl;
    cout << "3a\t" << f3a(getContent("./input/input-03.txt")) << endl;
    cout << "4\t" << f4(getContent("./input/input-04.txt")) << endl;
    cout << "4a\t" << f4a(getContent("./input/input-04.txt")) << endl;
    cout << "5\t" << f5(getContent("./input/input-05.txt")) << endl;
    cout << "5a\t" << f5a(getContent("./input/input-05.txt")) << endl;
    cout << "6\t" << f6(getContent("./input/input-06.txt")) << endl;
    cout << "6a\t" << f6a(getContent("./input/input-06.txt")) << endl;
    cout << "7\t" << f7(getContent("./input/input-07.txt")) << endl;
    cout << "7a\t" << f7a(getContent("./input/input-07.txt")) << endl;
    return 0;
}
