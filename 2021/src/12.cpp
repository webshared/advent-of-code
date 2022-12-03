#include "common.hpp"

int f12(vector<string> input, bool option2 = false) {
    map<string, set<string>> graph;
    auto add = [&graph](string from, string to) {
        if (to == "start") return;
        if (graph.count(from)) {
            graph[from].insert(to);
        } else {
            graph.insert(pair{from, set<string>{to}});
        }
    };
    for (auto line: input) {
        auto fromTo = split(line, '-');
        add(fromTo[0], fromTo[1]);
        add(fromTo[1], fromTo[0]);
    }
    vector<string> start{"start"};
    queue<vector<string>> q;
    q.push(start);
    int routes = 0;
    while(q.size()) {
        vector<string> path = q.front();
        q.pop();
        auto last = path.back();
        if (graph.count(last)) {
            set<string> dests; // vaild destination nodes
            set<string> exclude;
            bool excludeSmall = false;
            if (!option2) {
                // 1st part: exclude visited small caves
                excludeSmall = true;
            } else {
                // 2nd part:
                // if there is a small cave visited twice - exclude all visited small caves
                // otherwise - exclude nothing
                set<string> passedTwice;
                for (string node: path) {
                    if (node[0] >= 'a' && node[0] <= 'z' && passedTwice.count(node)) {
                        excludeSmall = true;
                        break;
                    }
                    passedTwice.insert(node);
                }
            }
            if (excludeSmall) {
                copy_if(
                    path.begin(), path.end(),
                    inserter(exclude, exclude.end()),
                    [](string node) { return node[0] >= 'a' && node[0] <= 'z'; }
                );
                set_difference(graph[last].begin(), graph[last].end(),
                    exclude.begin(), exclude.end(), inserter(dests, dests.end()));
            } else {
                dests = graph[last];
            }
            for (string dest: dests) {
                vector<string> newPath;
                newPath = path;
                newPath.push_back(dest);
                if (dest == "end") {
                    ++routes;
                    // for (string v: newPath) cout << v << ","; cout << endl;
                } else {
                    q.push(newPath);
                }
            }
        }
    }
    return routes;
}

int main() {
    cout << "12\t" << f12(getContent("./input/input-12.txt")) << endl;
    cout << "12a\t" << f12(getContent("./input/input-12.txt"), true) << endl;
    return 0;
}
