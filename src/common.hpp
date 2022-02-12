#ifndef COMMON_HPP
#define COMMON_HPP
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <queue>
#include <deque>
#include <set>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <chrono>
#include <boost/regex.hpp>

using namespace std;

vector<string> getContent(string const file) {
  if (access( file.c_str(), F_OK ) != 0) {
      cerr << "File " << file << " not found" << endl;
      exit(1);
  }
  ifstream input(file);
  vector<string> lines;
  if (input.is_open())
  {
    string line;
    while (getline(input, line))
    {
        if (!line.empty())
            lines.push_back(line);
    }
    input.close();
  }
  return lines;
}

// Helpers
vector<int> range(int from, int to, bool inclusive = false) {
    vector<int> vec;
    if (from <= to) {
      int toI = inclusive ? to: to - 1;
      for (int i = from; i <= toI; ++i) {
          vec.push_back(i);
      }
    } else {
      int toI = inclusive ? to : to + 1;
      for (int i = from; i >= to; --i) {
          vec.push_back(i);
      }
    }
    return vec;
}

template <typename T>
void logVec(const vector<T> &vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << "\t";
    }
    cout << endl;
}

int ones(int bits) {
  int res = 0;
  for (int i = 0; i < bits; i++) {
      res = ((res | 1) << 1);
  }
  res >>= 1;
  return res;
}

int fromBits(string binaryStr) {
    int res = 0;
    for (int i = 0; i < binaryStr.length(); i++) {
        res = res | (binaryStr[i] == '1' ? 1 : 0);
        if (i < binaryStr.length() - 1) {
            res <<= 1;
        }
    }
    return res;
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        if (!item.empty()) {
            *result++ = item;
        }
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::vector<int> splitToInts(const std::string &s, char delim) {
    std::vector<int> elems;
    for (string v: split(s, delim)) {
        if (!v.empty()) {
            try {
               elems.push_back(stoi(v));
            } catch(std::invalid_argument error) {}
        }
    }
    return elems;
}

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

#endif
