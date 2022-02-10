#include "common.hpp"

int f20(const vector<string> &input, int stepsCnt) {
    string decoder = input[0];
    vector<string> imgRaw;
    for (int i = 1; i < input.size(); i++)
        if (input[i].length())
            imgRaw.push_back(input[i]);
    int pad = stepsCnt * 2;
   
    vector<string> img;
    string paddingLine(imgRaw[0].length() + pad * 2, '.');
    string padding(pad, '.');
    for (int i = 0; i < pad; ++i)
        img.push_back(paddingLine);
    for (auto line: imgRaw)
        img.push_back(padding + line + padding);
    for (int i = 0; i < pad; ++i)
        img.push_back(paddingLine);

    auto prnImg = [] (const vector<string> &img) {
        for (auto line: img)
            cout << line << endl;
        cout << endl;
    };

    int lit = 0;
    int bgcolor = '.';
    for (int step = 0; step < stepsCnt; ++step) {
        vector<string> img2(img.size());
        for (int i = 0; i < img.size(); ++i)
            img2[i] = paddingLine;
        lit = 0 ;
        for (int i = 0; i < img.size(); ++i) {
            for (int j = 0; j < img[i].length(); ++j) {
                int bits = 0;
                for (int di = -1; di <= 1; ++di)
                    for (int dj = -1; dj <= 1; ++dj) {
                        char pixel = i + di >= 0 && i + di < img.size() && j + dj >= 0 && j + dj < img[i].length()
                            ? img[i + di][j + dj]
                            : bgcolor;
                        bits = (bits << 1) | (pixel == '#' ? 1 : 0);
                    }
                img2[i][j] = decoder[bits];
                lit += decoder[bits] == '#' ? 1 : 0;
            }
        }
        bgcolor = decoder[bgcolor == '.' ? 0 : 511];
        img = img2;
    }
    prnImg(img);
    return lit;
}

int main() {
    int a = f20(getContent("./input/input-20.txt"), 2);
    cout << "20\t" << a << endl;
    int b = f20(getContent("./input/input-20.txt"), 50);
    cout << "20a\t" << b << endl;
    return 0;
}
