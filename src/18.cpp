#include "common.hpp"

struct Node {
    Node *parent;
    bool isValue;
    Node *l;
    Node *r;
    int value;
};

void addToLeftNeib(Node *ptr, int value) {
    for(;ptr && ptr->parent; ptr = ptr->parent) {
        if (ptr->parent->l != ptr) {
            // look for "ptr->parent->l" rightmost leaf
            Node *leaf = ptr->parent->l;
            for (;leaf;) {
                if (leaf->isValue) {
                    leaf->value += value;
                    break;
                } else {
                    leaf = leaf->r;
                }
            }
            break;
        }
    }
}

void addToRightNeib(Node *ptr, int value) {
    for(;ptr && ptr->parent; ptr = ptr->parent) {
        if (ptr->parent->r != ptr) {
            Node *leaf = ptr->parent->r;
            for (;leaf;) {
                if (leaf->isValue) {
                    leaf->value += value;
                    break;
                } else {
                    leaf = leaf->l;
                }
            }
            break;
        }
    }
}

pair<const char*, Node*> parse(const char *str) {
    Node *n = new Node();
    n->parent = NULL;
    n->isValue = false;
    if (*str == '[') {
        auto[newStr, left] = parse(++str);
        left->parent = n;
        n->l = left;
        str = newStr;
    }
    if (*str == ',') {
        auto[newStr, right] = parse(++str);
        right->parent = n;
        n->r = right;
        str = newStr;
    }
    if (*str == ']') {
        return pair{++str, n};
    }
    if (*str >= '0' && *str <= '9') {
        n->isValue = true;
        n->value = (int)(*str - '0');
        return pair{str + 1, n};
    }
}

void _prn(Node *n) {
    if (n->isValue) {
        cout << n->value;
    } else {
        cout << "["; _prn(n->l); cout << ","; _prn(n->r); cout << "]";
    }
}

void prn(const char *label, Node *n) {
    cout << label << endl;
    _prn(n);
    cout << endl;
}

Node* tree(const char* str) {
    auto [foo, root] = parse(str);
    return root;
}

int magnitude(Node *n) {
    if (n->isValue) return n->value;
    return 3 * magnitude(n->l) + 2 * magnitude(n->r);
}

Node* snailplus(Node *a, Node *b) {
    Node *n = new Node;
    n->isValue = false;
    n->l = a;
    n->r = b;
    a->parent = n;
    b->parent = n;
    return n;
}

bool explode(Node *n, int level) {
    if (n->isValue) return false;
    if (level <= 3) {
        if (explode(n->l, level + 1)) return true;
        if (explode(n->r, level + 1)) return true;
        return false;
    }
    n->isValue = true;
    n->value = 0;
    addToLeftNeib(n, n->l->value);
    addToRightNeib(n, n->r->value);
    return true;
}

bool split(Node *n, int level) {
    if (n->isValue) {
        if (n->value >= 10) {
            n->isValue = false;
            n->l = new Node;
            n->l->parent = n;
            n->l->isValue = true;
            n->l->value = n->value / 2;
            n->r = new Node;
            n->r->parent = n;
            n->r->isValue = true;
            n->r->value = (n->value + 1) / 2;
            return true;
        }
        return false;
    }
    if (split(n->l, level + 1)) return true;
    if (split(n->r, level + 1)) return true;
    return false;
}

void reduce(Node *n) {
    for (;;) {
        if (!explode(n, 0) && !split(n, 0)) break;
    }
}

int f18(const vector<string> &input) {
    Node *sum = tree(input[0].c_str());
    // prn("FIRST", sum);
    for (int i = 1; i < input.size(); i++) {
        Node *cur = tree(input[i].c_str());
        // prn("CUR", cur);
        sum = snailplus(sum, cur);
        // prn("SUM", sum);
        reduce(sum);
        prn("SUM REDUCED", sum);
    }
    return magnitude(sum);
}

int f18a(const vector<string> &input) {
    int maxm = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input.size(); j++) {
            if (i != j) {
                Node *a = tree(input[i].c_str());
                Node *b = tree(input[j].c_str());
                Node *sum = snailplus(a, b);
                reduce(sum);
                int m = magnitude(sum);
                if (m > maxm) maxm = m;
                //prn("SUM REDUCED", sum);
            }
        }
    }
    return maxm;
}

int main() {
    cout << "18\t" << f18(getContent("./input/input-18.txt")) << endl;
    cout << "18a\t" << f18a(getContent("./input/input-18.txt")) << endl;
    return 0;
}
