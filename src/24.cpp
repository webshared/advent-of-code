#include "common.hpp"

typedef vector<int> regs;

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

int regIdx(string regName) {
    switch(regName[0]) {
        case 'w': return 0;
        case 'x': return 1;
        case 'y': return 2;
        case 'z': return 3;
        default: return -1;
    }
}

int op2(vector<string> &op, regs &reg) {
    return regIdx(op[2]) >= 0 ? reg[regIdx(op[2])] : stoi(op[2]);
}

ulong f24(const vector<string> &input) {
    map<regs, string> state;
    regs init = {0, 0, 0, 0};
    state[init] = "";

    auto addState = [] (map<regs, string> &state, const regs &reg, const string &path) {
        if (state.count(reg)) {
            state[reg] = state[reg] < path ? path : state[reg];
        } else {
            state.insert(pair{reg, path});
        }
    };

    int opIdx = 0, varIdx = 0;
    for (auto &line: input) {
        auto op = split(line, ' ');
        map<regs, string> newState;
        if (op[0] == "inp") {
            ++varIdx;
            for (const auto &[reg, path]: state) {
                regs r = reg;
                for (int j: range(1, 9, true)) {
                    r[regIdx(op[1])] = j;
                    addState(newState, r, path + string(1, '0' +j));
                }
            }
        } else {
            if (op[0] == "add") {
                for (auto &[reg, path]: state) {
                    regs r = reg;
                    r[regIdx(op[1])] += op2(op, r);
                    addState(newState, r, path);
                }
            } else if (op[0] == "mul") {
                for (auto &[reg, path]: state) {
                    regs r = reg;
                    r[regIdx(op[1])] *= op2(op, r);
                    addState(newState, r, path);
                }
            } else if (op[0] == "div") {
                for (auto &[reg, path]: state) {
                    regs r = reg;
                    r[regIdx(op[1])] /= op2(op, r);
                    addState(newState, r, path);
                }
            } else if (op[0] == "mod") {
                for (auto &[reg, path]: state) {
                    regs r = reg;
                    r[regIdx(op[1])] %= op2(op, r);
                    addState(newState, r, path);
                }
            } else if (op[0] == "eql") {
                for (auto &[reg, path]: state) {
                    regs r = reg;
                    r[regIdx(op[1])] = r[regIdx(op[1])] == op2(op, r) ? 1 : 0;
                    addState(newState, r, path);
                }
            }
        }
        state = newState;
        
        //for (const auto &[r, path]: state)
        //    cout << "w=" << r[0] << " x=" << r[1] << " y=" << r[2] << " z=" << r[3] << " path=" << path << endl;
        cout << ++opIdx << " varIdx=" << varIdx << " states: " << state.size() << endl;

        double vm, rss;
        process_mem_usage(vm, rss);
        cout << "VM: " << vm << "; RSS: " << rss << endl;
    }
    return 0;
}

int main() {
    ulong a = f24(getContent("./src/input-24.txt"));
    cout << "24\t" << a << endl;
    assert(a == 0);
    // ulong b = f24(getContent("./input/input-24.txt"), true);
    // cout << "24a\t" << b << endl;
    // assert(b == 48984);
    return 0;
}
