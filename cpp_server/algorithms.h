#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QString>
#include <QJsonObject>

class Algorithms
{
public:
    static QString sha512(const QString &text);
    static QString rsaDemoEncrypt(const QString &text);
    static QJsonObject bisection(double a, double b, int iterations = 50);
    static QJsonObject shortestPath(int from, int to);

private:
    static double functionForBisection(double x);
    static long long modPow(long long base, long long exp, long long mod);
};

#endif
