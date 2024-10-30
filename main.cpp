#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "json.hpp"
#include <fstream>

using namespace std;

struct Point {
    int x;
    long long y;
};

long long decodeValue(const string &value, int base) {
    long long result = 0;
    for (char c : value) {
        result = result * base + (isdigit(c) ? c - '0' : c - 'a' + 10);
    }
    return result;
}

long long interpolate(const vector<Point> &points, int k) {
    long long secret = 0;
    for (int i = 0; i < k; ++i) {
        long long term = points[i].y;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= points[j].x;
                term /= (points[j].x - points[i].x);
            }
        }
        secret += term;
    }
    return secret;
}

long long findConstantTerm(const string &filename) {
    ifstream file(filename);
    json::Value root;
    file >> root;

    int k = root["keys"]["k"].asInt();
    vector<Point> points;

    for (json::Value::const_iterator it = root.begin(); it != root.end(); ++it) {
        if (it.key().asString() != "keys") {
            int x = stoi(it.key().asString());
            int base = stoi((*it)["base"].asString());
            long long y = decodeValue((*it)["value"].asString(), base);
            points.push_back({x, y});
        }
    }

    return interpolate(points, k);
}

int main() {
    cout << "Secret for testcase 1: " << findConstantTerm("testcase1.json") << endl;
    cout << "Secret for testcase 2: " << findConstantTerm("testcase2.json") << endl;
    return 0;
}