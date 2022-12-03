#include "common.hpp"

pair<int, int> f17(const vector<string> &input) {
    using namespace boost;
    regex expression("x=([-\\d]+)..([-\\d]+), y=([-\\d]+)..([-\\d]+)");
    boost::match_results<std::string::const_iterator> what;
    if (regex_search(input[0].begin(), input[0].cend(), what, expression)) {
        int x0 = stoi(string(what[1].first, what[1].second));
        int x1 = stoi(string(what[2].first, what[2].second));
        int y0 = stoi(string(what[3].first, what[3].second));
        int y1 = stoi(string(what[4].first, what[4].second));
        cout << endl << "area x=" << x0 << "," << x1 << " y=" << y0 << "," << y1 << endl;
        int x = 0, y = 0, h = 0, hits = 0;
        for (int initVx: range(1, x1, true)) {
            for (int initVy: range(-y0, y0, true)) {
                int posX = x, posY = y;
                int maxH = y;
                int vx = initVx, vy = initVy;
                bool reached = false;
                for (;;) {
                    if (posY < y0 && vy <= 0) break; // too low
                    if (vx == 0 && (posX < x0 || posX > x1)) break; // outside X range
                    posX += vx;
                    posY += vy;
                    vx += (vx > 0) ? -1 : (vx < 0) ? 1 : 0;
                    vy -= 1;
                    if (posY > maxH) maxH = posY;
                    if (posX >= x0 && posX <= x1 && posY >= y0 && posY <= y1) {
                        reached = true;
                        break;
                    }
                }
                if (reached) {
                    if (maxH > h) h = maxH;
                    ++hits;
                    // cout << "vx=" << vx << " vy=" << vy << " maxH=" << maxH << " h=" << h << (reached ? " - HIT" : " - missed") << endl;
                }
            }
        }
        return pair{h, hits};
    }
    return pair{-1, -1};
}

int main() {
    auto [h, hits] = f17(getContent("./input/input-17.txt"));
    cout << "17\t" << h << endl;
    cout << "17a\t" << hits << endl;
    return 0;
}
