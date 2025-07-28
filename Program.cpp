#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

typedef long long ll;
typedef long double ld;

struct Point {
    ll x;
    ld y;
};

ll convertToDecimal(const string& value, int base) {
    ll result = 0;
    for (char ch : value) {
        ll digit;
        if (isdigit(ch)) digit = ch - '0';
        else digit = tolower(ch) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}

vector<Point> parseJsonFile(const string& filename, int k) {
    ifstream file(filename);
    json j;
    file >> j;

    vector<Point> points;
    for (auto& el : j.items()) {
        if (el.key() == "keys") continue;
        ll x = stoll(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string valStr = el.value()["value"].get<string>();
        ll y = convertToDecimal(valStr, base);
        points.push_back({x, static_cast<ld>(y)});
    }

    sort(points.begin(), points.end(), [](const Point& a, const Point& b) { return a.x < b.x; });
    return vector<Point>(points.begin(), points.begin() + k);
}

ld lagrangeInterpolationAtZero(const vector<Point>& points) {
    int k = points.size();
    ld result = 0.0;
    for (int i = 0; i < k; ++i) {
        ld term = points[i].y;
        for (int j = 0; j < k; ++j) {
            if (i == j) continue;
            term *= -points[j].x;
            term /= (points[i].x - points[j].x);
        }
        result += term;
    }
    return result;
}

int main() {
    vector<string> files = {"testcase1.json", "testcase2.json"};
    int testCase = 1;

    for (const string& file : files) {
        ifstream f(file);
        json j;
        f >> j;
        int k = j["keys"]["k"];
        vector<Point> points = parseJsonFile(file, k);
        ld secret = lagrangeInterpolationAtZero(points);
        cout << llround(secret) << endl;
        testCase++;
    }

    return 0;
}
