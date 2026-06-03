#include "algorithms.h"

#include <QCryptographicHash>
#include <QJsonArray>
#include <QStringList>
#include <cmath>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>

QString Algorithms::sha512(const QString &text)
{
    QByteArray hash = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha512);
    return QString(hash.toHex());
}

long long Algorithms::modPow(long long base, long long exp, long long mod)
{
    long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }

        base = (base * base) % mod;
        exp /= 2;
    }

    return result;
}

QString Algorithms::rsaDemoEncrypt(const QString &text)
{
    // Educational RSA demo: p = 61, q = 53, n = 3233, e = 17.
    // This is used only to demonstrate the algorithm.
    const long long n = 3233;
    const long long e = 17;

    QStringList encryptedValues;

    for (QChar ch : text) {
        int code = ch.unicode();
        long long encrypted = modPow(code, e, n);
        encryptedValues << QString::number(encrypted);
    }

    return encryptedValues.join(" ");
}

double Algorithms::functionForBisection(double x)
{
    // f(x) = x^3 - x - 2
    return x * x * x - x - 2.0;
}

QJsonObject Algorithms::bisection(double a, double b, int iterations)
{
    QJsonObject result;

    double fa = functionForBisection(a);
    double fb = functionForBisection(b);

    if (fa * fb > 0) {
        result["status"] = "error";
        result["message"] = "Function has the same signs at a and b";
        return result;
    }

    double mid = a;

    for (int i = 0; i < iterations; ++i) {
        mid = (a + b) / 2.0;
        double fm = functionForBisection(mid);

        if (std::abs(fm) < 0.000001) {
            break;
        }

        if (fa * fm < 0) {
            b = mid;
            fb = fm;
        } else {
            a = mid;
            fa = fm;
        }
    }

    result["status"] = "ok";
    result["method"] = "bisection";
    result["function"] = "x^3 - x - 2";
    result["root"] = mid;
    result["f_root"] = functionForBisection(mid);
    result["iterations"] = iterations;

    return result;
}

QJsonObject Algorithms::shortestPath(int from, int to)
{
    QJsonObject result;

    const int nodes = 5;

    if (from < 0 || from >= nodes || to < 0 || to >= nodes) {
        result["status"] = "error";
        result["message"] = "Invalid node number. Use nodes from 0 to 4.";
        return result;
    }

    std::vector<std::vector<std::pair<int, int>>> graph(nodes);

    graph[0].push_back({1, 4});
    graph[0].push_back({2, 2});
    graph[1].push_back({2, 1});
    graph[1].push_back({3, 5});
    graph[2].push_back({3, 8});
    graph[2].push_back({4, 10});
    graph[3].push_back({4, 2});
    graph[4].push_back({3, 2});

    const int INF = std::numeric_limits<int>::max();

    std::vector<int> dist(nodes, INF);
    std::vector<int> parent(nodes, -1);

    using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> queue;

    dist[from] = 0;
    queue.push({0, from});

    while (!queue.empty()) {
        int currentDistance = queue.top().first;
        int vertex = queue.top().second;
        queue.pop();

        if (currentDistance != dist[vertex]) {
            continue;
        }

        for (const auto &edge : graph[vertex]) {
            int next = edge.first;
            int weight = edge.second;

            if (dist[vertex] + weight < dist[next]) {
                dist[next] = dist[vertex] + weight;
                parent[next] = vertex;
                queue.push({dist[next], next});
            }
        }
    }

    if (dist[to] == INF) {
        result["status"] = "error";
        result["message"] = "Path not found";
        return result;
    }

    std::vector<int> path;

    for (int v = to; v != -1; v = parent[v]) {
        path.push_back(v);
    }

    std::reverse(path.begin(), path.end());

    QJsonArray pathArray;

    for (int v : path) {
        pathArray.append(v);
    }

    result["status"] = "ok";
    result["algorithm"] = "Dijkstra";
    result["from"] = from;
    result["to"] = to;
    result["distance"] = dist[to];
    result["path"] = pathArray;

    return result;
}
