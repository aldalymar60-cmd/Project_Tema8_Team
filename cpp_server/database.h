#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QJsonArray>

class Database
{
public:
    static Database &instance();

    bool init(const QString &databaseFileName);

    bool registerUser(const QString &login, const QString &password);
    bool checkLogin(const QString &login, const QString &password);

    bool saveOperation(const QString &operation,
                       const QString &inputData,
                       const QString &resultData,
                       const QString &login = "");

    QJsonArray history(int limit = 20);

private:
    Database();
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    bool createTables();
    QString hashPassword(const QString &password) const;

    QString m_connectionName;
};

#endif
