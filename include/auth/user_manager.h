#pragma once

#include <string>
#include <memory>
#include <QObject>
#include <QCryptographicHash>

namespace tictactoe {

struct User {
    int id;
    std::string username;
    std::string passwordHash;
    std::string salt;
    std::string createdAt;
};

class UserManager : public QObject {
    Q_OBJECT

public:
    explicit UserManager(QObject* parent = nullptr);
    ~UserManager() = default;

    // User authentication
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    void logoutUser();

    // User management
    bool isUserLoggedIn() const;
    const User& getCurrentUser() const;
    bool changePassword(const std::string& currentPassword, const std::string& newPassword);

signals:
    void userLoggedIn(const User& user);
    void userLoggedOut();
    void loginFailed(const std::string& error);
    void registrationFailed(const std::string& error);

private:
    std::string generateSalt() const;
    std::string hashPassword(const std::string& password, const std::string& salt) const;
    bool validatePassword(const std::string& password, const std::string& hash, const std::string& salt) const;

    User currentUser_;
    bool isLoggedIn_;
}; 