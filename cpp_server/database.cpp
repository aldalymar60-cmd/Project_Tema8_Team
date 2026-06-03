#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>

Database::Database()
{
    m_connectionName = "tema8_server_connection";
}

Database &Database::instance()
{
    static Database database;
    return database;
}

bool Database::init(const QString &databaseFileName)
{
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    db.setDatabaseName(databaseFileName);

    if (!db.open()) {
        qDebug() << "SQLite open error:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected:" << databaseFileName;

    return createTables();
}

bool Database::createTables()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    QSqlQuery query(db);

    bool usersOk = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "created_at TEXT NOT NULL"
        ")"
    );

    if (!usersOk) {
        qDebug() << "Create users table error:" << query.lastError().text();
        return false;
    }

    bool operationsOk = query.exec(
        "CREATE TABLE IF NOT EXISTS operations ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT,"
        "operation TEXT NOT NULL,"
        "input_data TEXT,"
        "result_data TEXT,"
        "created_at TEXT NOT NULL"
        ")"
    );

    if (!operationsOk) {
        qDebug() << "Create operations table error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database tables are ready.";
    return true;
}

QString Database::hashPassword(const QString &password) const
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512);
    return QString(hash.toHex());
}

bool Database::registerUser(const QString &login, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO users (login, password_hash, created_at) "
        "VALUES (:login, :password_hash, :created_at)"
    );

    query.bindValue(":login", login);
    query.bindValue(":password_hash", hashPassword(password));
    query.bindValue(":created_at", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "Register user error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::checkLogin(const QString &login, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    QSqlQuery query(db);
    query.prepare(
        "SELECT id FROM users "
        "WHERE login = :login AND password_hash = :password_hash"
    );

    query.bindValue(":login", login);
    query.bindValue(":password_hash", hashPassword(password));

    if (!query.exec()) {
        qDebug() << "Login check error:" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool Database::saveOperation(const QString &operation,
                             const QString &inputData,
                             const QString &resultData,
                             const QString &login)
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO operations (login, operation, input_data, result_data, created_at) "
        "VALUES (:login, :operation, :input_data, :result_data, :created_at)"
    );

    query.bindValue(":login", login);
    query.bindValue(":operation", operation);
    query.bindValue(":input_data", inputData);
    query.bindValue(":result_data", resultData);
    query.bindValue(":created_at", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "Save operation error:" << query.lastError().text();
        return false;
    }

    return true;
}

QJsonArray Database::history(int limit)
{
    QJsonArray array;

    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    QSqlQuery query(db);
    query.prepare(
        "SELECT login, operation, input_data, result_data, created_at "
        "FROM operations "
        "ORDER BY id DESC "
        "LIMIT :limit"
    );

    query.bindValue(":limit", limit);

    if (!query.exec()) {
        qDebug() << "History query error:" << query.lastError().text();
        return array;
    }

    while (query.next()) {
        QJsonObject object;
        object["login"] = query.value("login").toString();
        object["operation"] = query.value("operation").toString();
        object["input_data"] = query.value("input_data").toString();
        object["result_data"] = query.value("result_data").toString();
        object["created_at"] = query.value("created_at").toString();

        array.append(object);
    }

    return array;
}
