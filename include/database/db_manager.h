#pragma once

#include <string>
#include <memory>
#include <vector>
#include <QObject>
#include <QSqlDatabase>
#include "../auth/user_manager.h"

namespace tictactoe {

struct GameRecord {
    int id;
    int userId;
    std::string result;
    std::string moves;
    std::string timestamp;
};

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    // Database initialization
    bool initialize();
    void close();

    // User operations
    bool createUser(const User& user);
    bool getUserByUsername(const std::string& username, User& user);
    bool updateUserPassword(int userId, const std::string& newPasswordHash, const std::string& newSalt);

    // Game history operations
    bool saveGameRecord(const GameRecord& record);
    std::vector<GameRecord> getUserGameHistory(int userId);
    std::vector<GameRecord> getTopPlayers(int limit = 10);

signals:
    void databaseError(const std::string& error);
    void databaseInitialized();
    void databaseClosed();

private:
    bool createTables();
    bool createUsersTable();
    bool createGameHistoryTable();

    QSqlDatabase db_;
    bool isInitialized_;
}; 