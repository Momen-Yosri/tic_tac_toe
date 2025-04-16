#include "database/db_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

namespace tictactoe {

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
    , isInitialized_(false)
{
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::initialize()
{
    if (isInitialized_) {
        return true;
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::current().filePath("tictactoe.db");
    db_.setDatabaseName(dbPath);

    if (!db_.open()) {
        emit databaseError("Failed to open database: " + db_.lastError().text().toStdString());
        return false;
    }

    if (!createTables()) {
        emit databaseError("Failed to create tables");
        return false;
    }

    isInitialized_ = true;
    emit databaseInitialized();
    return true;
}

void DatabaseManager::close()
{
    if (db_.isOpen()) {
        db_.close();
    }
    emit databaseClosed();
}

bool DatabaseManager::createUser(const User& user)
{
    if (!isInitialized_) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash, salt, created_at) "
                 "VALUES (:username, :password_hash, :salt, :created_at)");
    query.bindValue(":username", QString::fromStdString(user.username));
    query.bindValue(":password_hash", QString::fromStdString(user.passwordHash));
    query.bindValue(":salt", QString::fromStdString(user.salt));
    query.bindValue(":created_at", QString::fromStdString(user.createdAt));

    if (!query.exec()) {
        emit databaseError("Failed to create user: " + query.lastError().text().toStdString());
        return false;
    }

    return true;
}

bool DatabaseManager::getUserByUsername(const std::string& username, User& user)
{
    if (!isInitialized_) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT id, username, password_hash, salt, created_at FROM users WHERE username = :username");
    query.bindValue(":username", QString::fromStdString(username));

    if (!query.exec() || !query.next()) {
        return false;
    }

    user.id = query.value("id").toInt();
    user.username = query.value("username").toString().toStdString();
    user.passwordHash = query.value("password_hash").toString().toStdString();
    user.salt = query.value("salt").toString().toStdString();
    user.createdAt = query.value("created_at").toString().toStdString();

    return true;
}

bool DatabaseManager::updateUserPassword(int userId, const std::string& newPasswordHash, const std::string& newSalt)
{
    if (!isInitialized_) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET password_hash = :password_hash, salt = :salt WHERE id = :id");
    query.bindValue(":password_hash", QString::fromStdString(newPasswordHash));
    query.bindValue(":salt", QString::fromStdString(newSalt));
    query.bindValue(":id", userId);

    if (!query.exec()) {
        emit databaseError("Failed to update password: " + query.lastError().text().toStdString());
        return false;
    }

    return true;
}

bool DatabaseManager::saveGameRecord(const GameRecord& record)
{
    if (!isInitialized_) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO game_history (user_id, result, moves, timestamp) "
                 "VALUES (:user_id, :result, :moves, :timestamp)");
    query.bindValue(":user_id", record.userId);
    query.bindValue(":result", QString::fromStdString(record.result));
    query.bindValue(":moves", QString::fromStdString(record.moves));
    query.bindValue(":timestamp", QString::fromStdString(record.timestamp));

    if (!query.exec()) {
        emit databaseError("Failed to save game record: " + query.lastError().text().toStdString());
        return false;
    }

    return true;
}

std::vector<GameRecord> DatabaseManager::getUserGameHistory(int userId)
{
    std::vector<GameRecord> history;
    if (!isInitialized_) {
        return history;
    }

    QSqlQuery query;
    query.prepare("SELECT id, user_id, result, moves, timestamp FROM game_history "
                 "WHERE user_id = :user_id ORDER BY timestamp DESC");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        emit databaseError("Failed to get game history: " + query.lastError().text().toStdString());
        return history;
    }

    while (query.next()) {
        GameRecord record;
        record.id = query.value("id").toInt();
        record.userId = query.value("user_id").toInt();
        record.result = query.value("result").toString().toStdString();
        record.moves = query.value("moves").toString().toStdString();
        record.timestamp = query.value("timestamp").toString().toStdString();
        history.push_back(record);
    }

    return history;
}

std::vector<GameRecord> DatabaseManager::getTopPlayers(int limit)
{
    std::vector<GameRecord> topPlayers;
    if (!isInitialized_) {
        return topPlayers;
    }

    QSqlQuery query;
    query.prepare("SELECT u.id as user_id, u.username, "
                 "COUNT(CASE WHEN gh.result = 'WIN' THEN 1 END) as wins, "
                 "COUNT(*) as total_games "
                 "FROM users u "
                 "LEFT JOIN game_history gh ON u.id = gh.user_id "
                 "GROUP BY u.id, u.username "
                 "ORDER BY wins DESC, total_games DESC "
                 "LIMIT :limit");
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        emit databaseError("Failed to get top players: " + query.lastError().text().toStdString());
        return topPlayers;
    }

    while (query.next()) {
        GameRecord record;
        record.userId = query.value("user_id").toInt();
        record.result = query.value("wins").toString().toStdString() + " wins";
        topPlayers.push_back(record);
    }

    return topPlayers;
}

bool DatabaseManager::createTables()
{
    return createUsersTable() && createGameHistoryTable();
}

bool DatabaseManager::createUsersTable()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS users ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "username TEXT UNIQUE NOT NULL,"
                 "password_hash TEXT NOT NULL,"
                 "salt TEXT NOT NULL,"
                 "created_at TEXT NOT NULL"
                 ")");

    if (!query.exec()) {
        emit databaseError("Failed to create users table: " + query.lastError().text().toStdString());
        return false;
    }

    return true;
}

bool DatabaseManager::createGameHistoryTable()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS game_history ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "user_id INTEGER NOT NULL,"
                 "result TEXT NOT NULL,"
                 "moves TEXT NOT NULL,"
                 "timestamp TEXT NOT NULL,"
                 "FOREIGN KEY (user_id) REFERENCES users(id)"
                 ")");

    if (!query.exec()) {
        emit databaseError("Failed to create game history table: " + query.lastError().text().toStdString());
        return false;
    }

    return true;
}

} // namespace tictactoe 