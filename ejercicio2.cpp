#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/HashCache.cpp"
#include <cstring>

using namespace std;
./scripts/compilation.sh
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    Cache cache(N);
    string op, domain, path, title;
    int time;

    for (int i = 0; i < N; ++i) {
        if (!(cin >> op)) return 0;

        if (op == "PUT") {
            cin >> domain >> path >> title >> time;
            cache.put(domain, path, title, time);
        } else if (op == "GET") {
            cin >> domain >> path;
            cache.get(domain, path);
        } else if (op == "CONTAINS") {
            cin >> domain >> path;
            cache.contains(domain, path);
        } else if (op == "COUNT_DOMAIN") {
            cin >> domain;
            cache.countDomain(domain);
        } else if (op == "LIST_DOMAIN") {
            cin >> domain;
            cache.listDomain(domain);
        } else if (op == "SIZE") {
            cache.size();
        } else if (op == "REMOVE") {
            cin >> domain >> path;
            cache.remove(domain, path);
        } else if (op == "CLEAR_DOMAIN") {
            cin >> domain;
            cache.clearDomain(domain);
        } else if (op == "CLEAR") {
            cache.clear();
        }
    }
    return 0;
}